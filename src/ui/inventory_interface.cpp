#include <ui/inventory_interface.hpp>

#include <SFML/Window/Mouse.hpp>

#include <resources/font_manager.hpp>

#include <util/fmouse.hpp>

#include <world/tile.hpp>

#include <util/vector2_stream.hpp>

// please don't tell anyone how i live

const float Inventory_Interface::cell_padding = Inventory_Cell::frameOutlineSize;

Inventory_Interface::Inventory_Interface(Player_Inventory& inventory, sf::View& view)
    : inventory { inventory }
    , view{ view }
{
    cells.push_back(std::vector<Inventory_Cell>());
    resize();
    readInventory();

    setEquippedIndex(0);

    dragCountText.setFont(Font_Manager::get(Font::UI));
    dragCountText.setFillColor(sf::Color::Black);

    progress_bar.setFillColor(sf::Color::White);
    progress_bar.setSize(sf::Vector2f(Tile::tileSize, 0.f));
    progress_bar.setOrigin(sf::Vector2f(Tile::tileSize / 2.f, Tile::tileSize / 2.f));
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

void Inventory_Interface::update(sf::RenderWindow& window)
{
    checkDrag();

    if (machine && machine->current_reaction >= 0) {
        setProgressBarSize();
        checkReaction();
    }

    checkTooltip(window);
}

void Inventory_Interface::checkTooltip(sf::RenderWindow& window)
{
    sf::Vector2i moused = mousedIndex(sf::Mouse::getPosition());
    if (active_tooltip) {
        if (reaction_tooltip) {
            std::shared_ptr<Tooltip> t = reaction_interface.findTooltip(fMouse(window, reaction_interface.getView()));
            if (t != active_tooltip) {
                active_tooltip = t;
            }
/*
            if (!active_tooltip && moused.x >= 0 && moused.y >= 0) {
                std::shared_ptr<Item> item = cells[moused.x][moused.y].getItem();
                if (item) {
                    active_tooltip = std::make_shared<Tooltip>(item);
                }
            }*/
        }
        else if (moused.x < 0 && moused.y < 0) {
            active_tooltip = reaction_interface.findTooltip(fMouse(window, reaction_interface.getView()));
        }
        else {
            if (tooltip_index != moused) {
                active_tooltip.reset();
                std::shared_ptr<Item> item = cells[moused.x][moused.y].getItem();
                if (item) {
                    active_tooltip = std::make_shared<Tooltip>(item);
                    tooltip_index = moused;
                }
            }
        }
        // disable tooltip if appropriate
    }
    else {
        if (moused.x >= 0 && moused.y >= 0) {
            std::shared_ptr<Item> item = cells[moused.x][moused.y].getItem();
            if (item) {
                active_tooltip = std::make_shared<Tooltip>(item);
                tooltip_index = moused;
            }
        }
        else {
            std::shared_ptr<Tooltip> t = reaction_interface.findTooltip(fMouse(window, reaction_interface.getView()));
            if (t) {
                active_tooltip = t;
                tooltip_index = moused;
            }
            // check reaction panels
        }
    }

    if (active_tooltip) {
        active_tooltip->setPosition(fMouse(), sf::Vector2f(window.getSize()));
    }
}

void Inventory_Interface::checkReaction()
{
    size_t r = inventory.rowCount;
    size_t n = cells[r].size();
    std::vector<std::shared_ptr<Item>> reagants = machine->activeReagants();
    for (size_t i = 0; i < n; i++) {
        cells[r][i].updateCount();
    }
    r++;
    if (!cells[r].front().getItem()) {
        cells[r].front().setItem(machine->activeProduct());
    }
    else {
        cells[inventory.rowCount + 1].front().updateCount();
    }
}

void Inventory_Interface::setProgressBarSize()
{
    sf::Vector2f size;
    size.x = Tile::tileSize;
    size.y = (Tile::tileSize * machine->reactionProgress());
    progress_bar.setSize(size);
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
            pos.x += Inventory_Cell::frameSize.x + cell_padding;
        }
        if (pos.x > max_x) {
            max_x = pos.x;
        }
        pos.x = x;
        if (r == 0) {
            pos.y -= Inventory_Cell::frameSize.y * 5.f;
        }
        else {
            pos.y += Inventory_Cell::frameSize.y + cell_padding;
        }
    }

    pos.x = max_x + 256.f;
    pos.y -= 512.f;

    sf::Vector2f size(416.f, 512.f);

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
    if (reaction_interface.scroll(delta, fMouse(window, reaction_interface.getView()))) {
        return false;
    }
    else if (delta > 0.f) {
        scrollLeft();
    }
    else if (delta < 0.f) {
        scrollRight();
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

void Inventory_Interface::close()
{
    if (dragging) {
        cancelDrag();
    }
    open = false;
    expanded = false;
    container = nullptr;
    machine = nullptr;
    clearProgressBar();

    reaction_interface.close();

    active_tooltip.reset();

    while (cells.size() > inventory.rowCount) {
        cells.pop_back();
    }
}

void Inventory_Interface::clickLeft(sf::RenderWindow& window)
{
    std::cout << "parsing click in inventory_interface!\n";
    if (machine) {
        sf::Vector2f mpos = fMouse(window, reaction_interface.getView());
        if (reaction_interface.contains(mpos)) {
            std::cout << "\treaction_interface click detected!\n";
            std::pair<Reaction*, std::shared_ptr<Item>> rxn = reaction_interface.click(fMouse(window, reaction_interface.getView()));
            Reaction* reaction = rxn.first;
            // move reagants in machine to inventory
            // move new reagants to machine
            // re-read the interface
        }
    }
    else if (crafting) {
        sf::Vector2f mpos = fMouse(window, reaction_interface.getView());
        if (reaction_interface.contains(mpos)) {
            std::cout << "\treaction_interface click detected!\n";
            std::pair<Reaction*, std::shared_ptr<Item>> rxn = reaction_interface.click(fMouse(window, reaction_interface.getView()));
            Reaction* reaction = rxn.first;
            if (reaction && rxn.second) {
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
                }
                return;
            }
        }
    }

    startDrag();
}

void Inventory_Interface::clickRight()
{}

void Inventory_Interface::startDrag()
{
    const sf::Vector2i mpos = sf::Mouse::getPosition();
    sf::Vector2i index = mousedIndex(mpos);
    if (index.x >= 0 && index.y >= 0) {
        sf::Vector2u i(index);
        std::shared_ptr<Item> item = cells[i.x][i.y].getItem();
        if (item) {
            dragItem = item;
            cells[i.x][i.y].clearItem();
            dragging = true;
            dragStartIndex = index;
            std::string text = "";
            if (dragItem->count() > 1) {
                text = std::to_string(dragItem->count());
            }
            dragCountText.setString(text);
            if (i.x < inventory.rowCount) {
                inventory.clearItem(i.x, i.y);
            }
            else if (container) {
                index.x -= inventory.rowCount;
                container->clearItem(index);
            }
            else if (machine) {
                if (i.x == inventory.rowCount) {
                    machine->clearReagant(i.y);
                    clearProgressBar();
                }
                else if (i.x == inventory.rowCount + 1) {
                    machine->clearProduct();
                }
            }
        }
    }
}

void Inventory_Interface::checkDrag()
{
    if (dragging && dragItem) {
        sf::Vector2f pos(sf::Mouse::getPosition());
        dragItem->setPosition(pos);

        pos.x -= Inventory_Cell::frameSize.x * .4f;
        dragCountText.setPosition(pos);
    }
}

void Inventory_Interface::endDrag(std::function<void(std::shared_ptr<Item>)> drop)
{
    dragging = false;
    if (!dragItem) {
        return;
    }
    const sf::Vector2i mpos = sf::Mouse::getPosition();
    sf::Vector2i i = mousedIndex(mpos);

    if (i.x >= 0 || i.y >= 0) {
        placeMergeSwap(i);
    }
    else {
        dragItem->can_pickup = false;
        drop(dragItem);
        dragItem.reset();
    }
}

void Inventory_Interface::placeMergeSwap(sf::Vector2i i)
{
    std::shared_ptr<Item> si = cells[i.x][i.y].getItem();
    if (i.x >= (int)inventory.rowCount) { // BUILDING PLACEMENT
        if (container) {
            if (si) {
                swap(i);
            }
            else {
                sf::Vector2i c(i);
                c.x -= inventory.rowCount;
                container->setItem(dragItem, c);
                dragItem.reset();
            }
            readBuilding();
        }
        else if (machine) {
            if (i.x == (int)inventory.rowCount) {
                size_t remainder = machine->addReagant(dragItem);
                if (remainder > 0) {
                    dragItem->setCount(remainder);
                    dragging = true;
                    return;
                }
                else {
                    dragItem = nullptr;
                }
                readBuilding();
            }
            else {
                dragging = true;
                return;
            }
        }
    } // END BUILDING PLACEMENT
    else { // INVENTORY PLACEMENT
        //if (dsi.x < (int)inventory.rowCount) {
            if (si) {
                if (si->getUID() == dragItem->getUID()) {
                    si->add(dragItem->count());
                    cells[i.x][i.y].setItem(si);
                    inventory.placeItem(i.x, i.y, si);
                    dragItem.reset();
                }
                else {
                    if (container || (machine && machine->validReagant(si->getName()))) {
                        swap(i);
                    }
                    else {
                        dragging = true;
                        return;
                    }
                }
            }
            else {
                cells[i.x][i.y].setItem(dragItem);
                inventory.placeItem(i.x, i.y, dragItem);
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

void Inventory_Interface::swap(sf::Vector2i i)
{
    sf::Vector2i dsi = dragStartIndex;
    cells[dsi.x][dsi.y].setItem(cells[i.x][i.y].getItem());
    cells[i.x][i.y].clearItem();
    cells[i.x][i.y].setItem(dragItem);
    dragItem.reset();

    if (dsi.x < (int)inventory.rowCount) {
        inventory.placeItem(dsi.x, dsi.y, cells[dsi.x][dsi.y].getItem());
    }
    else if (container) {
        sf::Vector2i c(dsi);
        c.x -= inventory.rowCount;
        container->setItem(cells[dsi.x][dsi.y].getItem(), c);
    }
    else if (machine) {
        machine->clearReagant(dsi.y);
        machine->addReagant(cells[dsi.x][dsi.y].getItem());
    }

    if (i.x < (int)inventory.rowCount) {
        inventory.placeItem(i.x, i.y, cells[i.x][i.y].getItem());
    }
    else if (container) {
        sf::Vector2i c(i);
        c.x -= inventory.rowCount;
        container->setItem(cells[i.x][i.y].getItem(), c);
    }
    else if (machine) {
        machine->clearReagant(i.y);
        machine->addReagant(cells[i.x][i.y].getItem());
    }
}

void Inventory_Interface::readBuilding()
{
    std::vector<std::vector<std::shared_ptr<Item>>>* items;
    if (container) {
        items = &container->getInventory();
    }
    else if (machine) {
        items = &machine->getInventory();
    }
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
    if (dragStartIndex.x >= (int)inventory.rowCount) {
        if (container) {
        }
        else if (machine) {
            if (dragStartIndex.x == (int)inventory.rowCount) {
                machine->addReagant(dragItem);
                if (dragItem) {
                    dragging = true;
                    return;
                }
            }
            else if (dragStartIndex.x == (int)inventory.rowCount + 1) {
                machine->setProduct(dragItem);
            }
        }
    }
    dragging = false;
    dragItem.reset();
}

sf::Vector2i Inventory_Interface::mousedIndex(const sf::Vector2i& mpos)
{
    size_t nr = cells.size();
    size_t nc;

    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();
        for (size_t c = 0; c < nc; c++) {
            if (cells[r][c].contains(mpos)) {
                return sf::Vector2i(r, c);
            }
        }
    }

    return sf::Vector2i(-1, -1);
}

void Inventory_Interface::loadBuilding(Building* b, Item_Library& item_library)
{
    while (cells.size() > inventory.rowCount) {
        cells.pop_back();
    }
    if (b && b->interface) {
        bool is_machine = false;
        switch(b->type) {
            case Building::CONTAINER:
                container = dynamic_cast<Container*>(b);
                break;
            case Building::MACHINE:
                is_machine = true;
                machine = dynamic_cast<Machine*>(b);
                if (machine->current_reaction >= 0) {
                    setProgressBarSize();
                }
                reaction_interface.load(b->reactions, inventory, item_library);
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
        pos.x += (Tile::tileSize * 2.f);
        float sx = pos.x;
        sf::Vector2f progress_pos = pos;
        progress_pos.y += (Tile::tileSize);
        progress_bar.setPosition(progress_pos);
        for (const auto& row : inventory) {
            pos.x = sx;
            cells.push_back(std::vector<Inventory_Cell>());
            for (const auto& item : row) {
                cells.back().push_back(Inventory_Cell(item));
                cells.back().back().setPosition(pos);
                pos.x += Tile::tileSize;
            }

            pos.y += Tile::tileSize * (1 + is_machine);
        }
    }
}

void Inventory_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    size_t nr = cells.size();
    size_t nc;
    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();\
        for (size_t c = 0; (c < nc) && (open || r == 0); c++) {
            target.draw(cells[r][c], states);
            if (r >= inventory.rowCount) {
            }
        }
    }

    if (crafting || machine) {
        target.draw(reaction_interface, states);
        target.setView(view);
    }

    if (machine && machine->current_reaction >= 0) {
        target.draw(progress_bar, states);
    }

    if (active_tooltip) {
        target.draw(*active_tooltip, states);
    }

    if (dragging && dragItem) {
        target.draw(*dragItem, states);
        target.draw(dragCountText, states);
    }
}
