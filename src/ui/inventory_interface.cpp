#include <ui/inventory_interface.hpp>

#include <SFML/Window/Mouse.hpp>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>

#include <util/fmouse.hpp>

#include <world/tile.hpp>

#include <util/vector2_stream.hpp>

// please don't tell anyone how i live

const float Inventory_Interface::cell_padding = Inventory_Cell::frameOutlineSize;

Inventory_Interface::Inventory_Interface(Player_Inventory& inventory, sf::View& view)
    : inventory { inventory }
    , view{ view }
{
    cells.clear();
    cells.push_back(std::vector<Inventory_Cell>());
    resize();
    readInventory();

    setEquippedIndex(0);

    dragCountText.setFont(Font_Manager::get(Font::UI));
    dragCountText.setFillColor(Palette::black);
    dragCountText.setOutlineColor(Palette::white);
    dragCountText.setOutlineThickness(2.f);
    dragCountText.setCharacterSize(Inventory_Cell::character_size);

    progress_bar.setFillColor(Palette::white);
    progress_bar.setSize(sf::Vector2f(Inventory_Cell::size, 0.f));
    progress_bar.setOrigin(sf::Vector2f(Inventory_Cell::size / 2.f, Inventory_Cell::size / 2.f));

    frame.setFillColor(Palette::inventory_bg);
    frame.setOutlineThickness(1.f);
    frame.setOutlineColor(Palette::inventory_outline);
    frame.setSize(sf::Vector2f(0.f, 0.f));
}

void Inventory_Interface::readInventory()
{
    size_t nr = inventory.rowCount;
    size_t nc = inventory.rowWidth;

    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].clearItem();
            cells[r][c].setItem(inventory(r, c));
        }
    }
}

void Inventory_Interface::writeInventory()
{
    size_t nr = inventory.rowCount;
    size_t nc = inventory.rowWidth;

    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].updateCount();
            inventory.placeItem(r, c, cells[r][c].getItem());
        }
    }
}

void Inventory_Interface::update(sf::RenderWindow& window)
{
    mousedIndex();
    pollChanges();
    checkDrag();
    checkTooltip(window);
}

void Inventory_Interface::checkTooltip(sf::RenderWindow& window)
{
    if (active_tooltip) {
        if (reaction_tooltip) {
            std::shared_ptr<Tooltip> t = reaction_interface.findTooltip(fMouse(window, reaction_interface.getView()));
            if (t != active_tooltip) {
                active_tooltip = t;
            }
/*
            if (!active_tooltip && moused_index.x >= 0 && moused_index.y >= 0) {
                std::shared_ptr<Item> item = cells[moused_index.x][moused_index.y].getItem();
                if (item) {
                    active_tooltip = std::make_shared<Tooltip>(item);
                }
            }*/
        }
        else if (moused_index.x < 0 && moused_index.y < 0) {
            active_tooltip = reaction_interface.findTooltip(fMouse(window, reaction_interface.getView()));
        }
        else {
            if (tooltip_index != moused_index) {
                active_tooltip.reset();
                std::shared_ptr<Item> item = cells[moused_index.x][moused_index.y].getItem();
                if (item) {
                    active_tooltip = std::make_shared<Tooltip>(item);
                    tooltip_index = moused_index;
                }
            }
        }
        // disable tooltip if appropriate
    }
    else {
        if (moused_index.x >= 0 && moused_index.y >= 0) {
            std::shared_ptr<Item> item = cells[moused_index.x][moused_index.y].getItem();
            if (item) {
                active_tooltip = std::make_shared<Tooltip>(item);
                tooltip_index = moused_index;
            }
        }
        else {
            std::shared_ptr<Tooltip> t = reaction_interface.findTooltip(fMouse(window, reaction_interface.getView()));
            if (t) {
                active_tooltip = t;
                tooltip_index = moused_index;
            }
            // check reaction panels
        }
    }

    if (active_tooltip) {
        active_tooltip->setPosition(fMouse(), sf::Vector2f(window.getSize()));
    }
}

void Inventory_Interface::clearProgressBar()
{
    sf::Vector2f size(0.f, 0.f);
    progress_bar.setSize(size);
}

void Inventory_Interface::resize()
{
    cells.resize(inventory.rowCount);

    for (auto& row : cells) {
        row.resize(inventory.rowWidth);
    }

    placeCells();
}

void Inventory_Interface::placeCells()
{
    float x = 96.f;
    sf::Vector2f pos(x, screenHeight - x);
    size_t nr = cells.size();
    size_t nc;
    float max_x = 0.f;
    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].setPosition(pos);
            pos.x += Inventory_Cell::size + cell_padding;
        }
        if (pos.x > max_x) {
            max_x = pos.x;
        }
        pos.x = x;
        if (r == 0) {
            pos.y -= Inventory_Cell::size * 5.f;
        }
        else {
            pos.y += Inventory_Cell::size + cell_padding;
        }
    }

    pos.x = max_x + 64.f;
    pos.y -= 512.f;

    sf::Vector2f size(320.f, 512.f);
    reaction_interface.setView(pos, size);
}

void Inventory_Interface::pollChanges()
{
    if (inventory.changed) {
        readInventory();
        inventory.changed = false;
    }
}

void Inventory_Interface::setEquippedIndex(size_t active)
{
    if (active >= SIZE_MAX) {
        active = cells[0].size() - 1;
    }
    if (active >= cells[0].size()) {
        active = 0;
    }

    cells[0][equippedIndex].deactivate();
    equippedIndex = active;
    cells[0][equippedIndex].activate();
    inventory.setEquipped(equippedIndex);
}

size_t Inventory_Interface::getEquippedIndex()
{
    return equippedIndex;
}

bool Inventory_Interface::scroll(float delta, sf::RenderWindow& window)
{
    if (open) {
        reaction_interface.scroll(delta, fMouse(window, reaction_interface.getView()));
        return false;
    }
    else {
        if (delta > 0.f) {
            scrollLeft();
        }
        else if (delta < 0.f) {
            scrollRight();
        }
    }
    return true;
}

void Inventory_Interface::scrollLeft()
{
    if (equippedIndex == 0) {
        setEquippedIndex(cells[0].size() - 1);
    }
    else {
        setEquippedIndex(equippedIndex - 1);
    }
}

void Inventory_Interface::scrollRight()
{
    setEquippedIndex(equippedIndex + 1);
}

void Inventory_Interface::close(std::function<void(std::shared_ptr<Item>)> drop)
{
    if (dragging) {
        if (dragStartIndex.x >= 0 && dragStartIndex.y >= 0) {
            cancelDrag();
        }
        else {
            drop(dragItem);
            dragItem.reset();
        }
    }
    open = false;
    expanded = false;
    container = nullptr;
    clearProgressBar();

    reaction_interface.close();

    active_tooltip.reset();
}

void Inventory_Interface::clickLeft(sf::RenderWindow& window)
{
    std::cout << "parsing click in inventory_interface!\n";
    sf::Vector2f mpos = fMouse(window, reaction_interface.getView());
    if (reaction_interface.contains(mpos)) {
        std::cout << "\treaction_interface click detected!\n";
        std::pair<Reaction*, std::shared_ptr<Item>> rxn = reaction_interface.click(fMouse(window, reaction_interface.getView()));
        Reaction* reaction = rxn.first;
        if (reaction) {
            Item product = *rxn.second;
            std::cout << "\tvalid reaction and product returned!\n";
            auto remove = [&]()
                {
                    for (const auto& r : reaction->reagants) {
                        inventory.removeItem(r.name, r.count);
                    }
                    readInventory();
                };

            if (dragItem) {
                if (dragItem->getUID() == product.getUID()) {
                    dragItem->add(product.count());
                    remove();
                    reaction_interface.check(inventory);
                }
            }
            else {
                dragItem = std::make_shared<Item>(product);
                dragging = true;
                checkDrag();
                remove();
                reaction_interface.check(inventory);
                dragStartIndex = sf::Vector2i(-1, -1);
            }
        }
    }
    else {
        startDrag();
    }
}

void Inventory_Interface::clickRight(std::function<void(std::shared_ptr<Item>)> drop)
{
    if (dragging) {
        if (moused_index.x < 0) {
            std::shared_ptr<Item> dropping = std::make_shared<Item>(*dragItem);
            dropping->setCount(1);
            dragItem->take(1);
            if (dragItem->count() == 0) {
                dragItem.reset();
                dragging = false;
            }
            drop(dropping);
            updateDragText();
            writeDecision();
        }
        else {
            std::shared_ptr<Item> i = cells[moused_index.x][moused_index.y].getItem();
            if (i) {
                if (i->getUID() == dragItem->getUID()
                && (int)(i->count() + 1) < i->stackSize()) {
                        dragItem->take(1);
                        if (dragItem->count() == 0) {
                            dragItem = nullptr;
                            dragging = false;
                        }
                        else {
                            updateDragText();
                        }
                        i->add(1);
                        if (moused_index.x >= (int)inventory.rowCount) {
                            writeExtension();
                        }
                        else {
                            writeInventory();
                        }
                        writeDecision();
                    }
                }
            else {
                std::shared_ptr<Item> placing = std::make_shared<Item>(*dragItem);
                placing->setCount(1);
                dragItem->take(1);
                cells[moused_index.x][moused_index.y].setItem(placing);
                if (dragItem->count() == 0) {
                    dragItem.reset();
                    dragging = false;
                }
                else {
                    updateDragText();
                }
                writeDecision();
            }
        }
    }
    else {
        std::shared_ptr<Item> i = cells[moused_index.x][moused_index.y].getItem();
        if (i) {
            dragging = true;
            dragItem = std::make_shared<Item>(*i);
            float intermediate = i->count();
            intermediate /= 2.f;
            intermediate += 0.9f; //aggressive round up to take the bigger half of odd numbers
            size_t diff = intermediate;
            i->take(diff);
            dragItem->setCount(diff);
            updateDragText();
            writeDecision();
        }
    }
}

void Inventory_Interface::writeDecision()
{
    if (moused_index.x >= (int)inventory.rowCount) {
        writeExtension();
    }
    else if (moused_index.x >= 0) {
        writeInventory();
    }
}

void Inventory_Interface::updateDragText()
{
    std::string text = "";
    if (dragItem->count() > 1) {
        text = std::to_string(dragItem->count());
    }
    dragCountText.setString(text);
}

void Inventory_Interface::startDrag()
{
    if (moused_index.x >= 0 && moused_index.y >= 0) {
        sf::Vector2u i(moused_index);
        std::shared_ptr<Item> item = cells[i.x][i.y].getItem();
        if (item) {
            dragItem = item;
            cells[i.x][i.y].clearItem();
            dragging = true;
            dragStartIndex = moused_index;
            updateDragText();
            if (i.x < inventory.rowCount) {
                inventory.clearItem(i.x, i.y);
            }
            else if (container) {
                moused_index.x -= inventory.rowCount;
                container->clearItem(moused_index);
            }
            writeDecision();
        }
    }
}

void Inventory_Interface::checkDrag()
{
    if (dragging && dragItem) {
        sf::Vector2f pos(sf::Mouse::getPosition());
        dragItem->setPosition(pos);

        pos.x -= Inventory_Cell::size * .45f;
        pos.y += Inventory_Cell::size * .45f;
        pos.y -= (Inventory_Cell::character_size + 2);
        dragCountText.setPosition(pos);
    }
}

void Inventory_Interface::endDrag(std::function<void(std::shared_ptr<Item>)> drop)
{
    dragging = false;
    if (!dragItem) {
        return;
    }

    if (moused_index.x >= 0 || moused_index.y >= 0) {
        placeMergeSwap();
        if (moused_index.x >= (int)inventory.rowCount) {
            writeExtension();
        }
    }
    else {
        dragItem->can_pickup = false;
        drop(dragItem);
        dragItem.reset();
    }
}

void Inventory_Interface::placeMergeSwap()
{
    std::shared_ptr<Item> si = cells[moused_index.x][moused_index.y].getItem();
    if (moused_index.x >= (int)inventory.rowCount) { // BUILDING PLACEMENT
        if (container) {
            if (si) {
                swap();
            }
            else {
                sf::Vector2i c(moused_index);
                c.x -= inventory.rowCount;
                container->setItem(dragItem, c);
                dragItem.reset();
            }
            readBuilding();
        }
    } // END BUILDING PLACEMENT
    else { // INVENTORY PLACEMENT
        //if (dsi.x < (int)inventory.rowCount) {
            if (si) {
                if (si->getUID() == dragItem->getUID()) { // merge
                    size_t remainder = si->add(dragItem->count());
                    cells[moused_index.x][moused_index.y].updateCount();
                    if (remainder == 0) {
                        dragItem = nullptr;
                    }
                    else {
                        dragItem->setCount(remainder);
                        dragging = true;
                        updateDragText();
                        return;
                    }
                    inventory.clearItem(moused_index.x, moused_index.y);
                    inventory.placeItem(moused_index.x, moused_index.y, si);
                } // end merge
                else {
                    if (dragStartIndex.x < 0 && dragStartIndex.y < 0) {
                        dragging = true;
                        return;
                    }
                    swap();
                }
            }
            else {
                cells[moused_index.x][moused_index.y].setItem(dragItem);
                inventory.placeItem(moused_index.x, moused_index.y, dragItem);
                dragItem.reset();
            }
        /*}
        else if (container) {
        }
        else if (machine) {
            return;
            if (i.x == (int)cells.size() - 1) { // reject product slot placement
                dragging = true;
            }
            else {
                if (si) {
                    dragging = true;
                    return;
                    if (si->getUID() == dragItem->getUID()) {
                        si->add(dragItem->count());
                        cells[i.x][i.y].setItem(si);
                        inventory.placeItem(i.x, i.y, si);
                        dragItem.reset();
                        dragItem = nullptr;
                        readBuilding();
                    }
                    if (machine->validReagant(si->getName())) {
                        swap(i);
                    }
                    else {
                        dragging = true;
                    }
                }
                else {
                    cells[i.x][i.y].setItem(dragItem);
                    inventory.placeItem(i.x, i.y, dragItem);
                    dragItem.reset();
                    dragItem = nullptr;
                }
            }
        }*/
    } // END INVENTORY PLACEMENT
}

void Inventory_Interface::swap()
{

    cells[dragStartIndex.x][dragStartIndex.y].setItem(cells[moused_index.x][moused_index.y].getItem());
    cells[moused_index.x][moused_index.y].clearItem();
    cells[moused_index.x][moused_index.y].setItem(dragItem);
    dragItem.reset();

    int rows = inventory.rowCount;

    if (dragStartIndex.x < rows) {
        inventory.placeItem(dragStartIndex.x, dragStartIndex.y, cells[dragStartIndex.x][dragStartIndex.y].getItem());
    }
    else if (container) {
        sf::Vector2i c(dragStartIndex);
        c.x -= inventory.rowCount;
        container->setItem(cells[dragStartIndex.x][dragStartIndex.y].getItem(), c);
    }

    if (moused_index.x < rows) {
        inventory.placeItem(moused_index.x, moused_index.y, cells[moused_index.x][moused_index.y].getItem());
    }
    else if (container) {
        sf::Vector2i c(moused_index);
        c.x -= inventory.rowCount;
        container->setItem(cells[moused_index.x][moused_index.y].getItem(), c);
    }
}

void Inventory_Interface::readBuilding()
{
    std::vector<std::vector<std::shared_ptr<Item>>>* items;
    if (container) {
        items = &container->getInventory();
    }
    //else if (machine) {
    //    items = &machine->getInventory();
    //}
    else { [[unlikely]]
        return;
    }

    size_t r = items->size();
    size_t n = inventory.rowCount;
    for (size_t i = 0; i < r; i++) {
        size_t c = (*items)[i].size();
        for (size_t j = 0; j < c; j++) {
            cells[i + n][j].clearItem();
            cells[i + n][j].setItem((*items)[i][j]);
        }
    }
}

void Inventory_Interface::cancelDrag()
{
    cells[dragStartIndex.x][dragStartIndex.y].setItem(dragItem);
    if (dragStartIndex.x < (int)inventory.rowCount) {
        inventory.placeItem(dragStartIndex.x, dragStartIndex.y, dragItem);
    }
    else {
        writeExtension();
    }
    dragging = false;
    dragItem.reset();
}

void Inventory_Interface::mousedIndex()
{
    const sf::Vector2f mpos = fMouse();
    size_t nr = cells.size();
    size_t nc;

    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();
        for (size_t c = 0; c < nc; c++) {
            if (cells[r][c].contains(mpos)) {
                moused_index = sf::Vector2i(r, c);
                return;
            }
        }
    }

    moused_index = sf::Vector2i(-1, -1);
}

void Inventory_Interface::loadReactions(std::vector<Reaction> reactions, Item_Library& item_library)
{
    reaction_interface.load(reactions, inventory, item_library);
}

void Inventory_Interface::loadBuilding(Building* b, Item_Library& item_library)
{
    while (cells.size() > inventory.rowCount) {
        cells.pop_back();
    }
    if (b && b->interface) {
        switch(b->type) {
            case Building::CONTAINER:
                container = dynamic_cast<Container*>(b);
                break;
            case Building::CRAFTING:
                crafting = dynamic_cast<Crafting*>(b);
                reaction_interface.load(b->reactions, inventory, item_library);
                break;
            default:
                return;
        }

        std::vector<std::vector<std::shared_ptr<Item>>>& inventory = b->getInventory();

        sf::Vector2f pos = cells[1].back().getPosition();
        pos.x += (Inventory_Cell::size * 2.f);
        float sx = pos.x;
        sf::Vector2f progress_pos = pos;
        progress_pos.y += (Inventory_Cell::size);
        progress_bar.setPosition(progress_pos);
        for (const auto& row : inventory) {
            pos.x = sx;
            cells.push_back(std::vector<Inventory_Cell>());
            for (const auto& item : row) {
                cells.back().push_back(Inventory_Cell(item));
                cells.back().back().setPosition(pos);
                pos.x += Inventory_Cell::size;
            }

            pos.y += Inventory_Cell::size;
        }
    }
}

void Inventory_Interface::readExtension()
{}

void Inventory_Interface::writeExtension()
{}

void Inventory_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (open) {
        target.draw(frame, states);

        target.draw(reaction_interface, states);
        target.setView(view);

        target.draw(progress_bar, states);
    }

    size_t nr = cells.size();
    size_t nc;
    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();
        for (size_t c = 0; (c < nc) && (open || r == 0); c++) {
            target.draw(cells[r][c], states);
            if (r >= inventory.rowCount) {
            }
        }
    }

    if (open) {
        if (active_tooltip) {
            target.draw(*active_tooltip, states);
        }

        if (dragging && dragItem) {
            target.draw(*dragItem, states);
            target.draw(dragCountText, states);
        }
    }
}
