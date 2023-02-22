#include <ui/inventory_interface.hpp>

#include <SFML/Window/Mouse.hpp>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>
#include <resources/texture_manager.hpp>

#include <util/fmouse.hpp>

#include <world/tile.hpp>

#include <util/vector2_stream.hpp>

// please don't tell anyone how i live

std::function<void(std::shared_ptr<Item>)> Inventory_Interface::drop = [](std::shared_ptr<Item>){};
size_t Inventory_Interface::equippedIndex = 0;

const float Inventory_Interface::cell_padding = Inventory_Cell::frameOutlineSize;

Inventory_Interface::Inventory_Interface(Player_Inventory& inventory, sf::View& view)
    : inventory { inventory }
    , view{ view }
{
    cells.clear();
    cells.push_back(std::vector<Inventory_Cell>());
    resize();
    readInventory();

    dragCountText.setFont(Font_Manager::get(Font::UI));
    dragCountText.setFillColor(Palette::black);
    dragCountText.setOutlineColor(Palette::white);
    dragCountText.setOutlineThickness(2.f);
    dragCountText.setCharacterSize(Inventory_Cell::character_size);

    progress_bar.setFillColor(Palette::white);
    progress_bar.setSize(sf::Vector2f(Inventory_Cell::size, 0.f));
    progress_bar.setOrigin(sf::Vector2f(Inventory_Cell::size / 2.f, Inventory_Cell::size / 2.f));

    progress_arrow.setSize(sf::Vector2f(0.f, 0.f));
    progress_arrow.setFillColor(Palette::inventory_bg);
    progress_arrow.setTexture(&Texture_Manager::get("UI"));
    progress_arrow.setTextureRect(sf::IntRect(64, 0, 64, 64));

    frame.setFillColor(Palette::inventory_bg);
    frame.setOutlineThickness(1.f);
    frame.setOutlineColor(Palette::inventory_outline);
    frame.setSize(sf::Vector2f(0.f, 0.f));

    setEquippedIndex(equippedIndex);
}

void Inventory_Interface::loadDrop(std::function<void(std::shared_ptr<Item>)> drop)
{
    this->drop = drop;
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
    checkDrag();
    checkTooltip(window);
    readInventory();
}

void Inventory_Interface::checkTooltip(sf::RenderWindow& window)
{
    if (!reaction_tooltip && moused != tooltip_index) {
        active_tooltip.reset();
        tooltip_index = moused;
        if (mousedItem()) {
            active_tooltip = std::make_shared<Tooltip>(mousedItem());
            reaction_tooltip = false;
        }
    }
    else if (moused.x < 0) {
        std::shared_ptr<Tooltip> t = reaction_interface.findTooltip(fMouse(window, reaction_interface.getView()));
        if (t != active_tooltip) {
            active_tooltip.reset();
            tooltip_index = moused;
            active_tooltip = t;
            reaction_tooltip = t.get();
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

void Inventory_Interface::close()
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
    clearProgressBar();

    reaction_interface.close();

    active_tooltip.reset();
}

void Inventory_Interface::clickLeft(sf::RenderWindow& window)
{
    std::cout << "parsing click in inventory_interface!\n";
    if (!checkReactionInterface(window)) {
        if (!dragging) {
            startDrag();
        }
        else {
            endDrag();
        }
    }
}

bool Inventory_Interface::checkReactionInterface(sf::RenderWindow& window)
{
    sf::Vector2f mpos = fMouse(window, reaction_interface.getView());
    bool parsed = reaction_interface.contains(mpos);
    if (parsed) {
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

            if (dragItem && dragItem->getUID() == product.getUID()) {
                dragItem->add(product.count());
                remove();
                reaction_interface.check(inventory);
                updateDragText();
            }
            else {
                dragItem = std::make_shared<Item>(product);
                dragging = true;
                checkDrag();
                remove();
                reaction_interface.check(inventory);
                dragStartIndex = sf::Vector2i(-1, -1);
                updateDragText();
            }
        }
    }
    return parsed;
}

void Inventory_Interface::clickRight()
{
    if (dragging) {
        if (moused.x < 0) {
            std::shared_ptr<Item> dropping = std::make_shared<Item>(*dragItem);
            dropping->setCount(1);
            dragItem->take(1);
            if (dragItem->count() == 0) {
                dragItem.reset();
                dragging = false;
            }
            drop(dropping);
            updateDragText();
        }
        else {
            if (mousedItem()) {
                if (mousedItem()->getUID() == dragItem->getUID()
                && (int)(mousedItem()->count() + 1) < mousedItem()->stackSize()) {
                        dragItem->take(1);
                        if (dragItem->count() == 0) {
                            dragItem = nullptr;
                            dragging = false;
                        }
                        mousedItem()->add(1);
                    }
                }
            else {
                std::shared_ptr<Item> placing = std::make_shared<Item>(*dragItem);
                placing->setCount(1);
                dragItem->take(1);
                mousedCell()->setItem(placing);
                if (dragItem->count() == 0) {
                    dragItem.reset();
                    dragging = false;
                }
            }
            writeInventory();
            writeExtension();
            updateDragText();
        }
    }
    else {
        if (mousedItem()) {
            dragging = true;
            dragItem = std::make_shared<Item>(*mousedItem());
            float intermediate = mousedItem()->count();
            intermediate /= 2.f;
            intermediate += 0.9f; //aggressive round up to take the bigger half of odd numbers
            size_t diff = intermediate;
            mousedItem()->take(diff);
            dragItem->setCount(diff);
            writeInventory();
            writeExtension();
            updateDragText();
        }
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
    dragStartIndex = moused;
    dragItem = mousedItem();
    updateDragText();
    dragging = true;
    if (dragItem && moused.x >= 0) {
        mousedCell()->clearItem();
        writeInventory();
        writeExtension();
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

void Inventory_Interface::endDrag()
{
    if (!dragItem) {
        dragging = false;
        return;
    }

    if (moused.x >= 0 || moused.y >= 0) {
        placeMergeSwap();
    }
    else {
        dragItem->can_pickup = false;
        drop(dragItem);
        dragItem.reset();
    }
}

void Inventory_Interface::placeMergeSwap()
{
    if (mousedItem()) {
        if (mousedItem()->getUID() == dragItem->getUID()) {
            merge();
        }
        else {
            swap();
        }
    }
    else {
        mousedCell()->setItem(dragItem);
        dragItem.reset();
        dragging = false;
    }

    writeInventory();
    writeExtension();
    tooltip_index = sf::Vector2i(-1, -1);
}

void Inventory_Interface::swap()
{
    cells[dragStartIndex.x][dragStartIndex.y].setItem(mousedItem());
    mousedCell()->clearItem();
    mousedCell()->setItem(dragItem);
    dragItem.reset();
    dragging = false;
}

void Inventory_Interface::merge()
{
    int sum = dragItem->count() + mousedItem()->count();
    if (sum > dragItem->stackSize()) {
        size_t diff = sum - dragItem->stackSize();
        mousedItem()->add(diff);
        dragItem->take(diff);
        updateDragText();
    }
    else {
        mousedItem()->add(dragItem->count());
        dragItem.reset();
        dragging = false;
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
                moused = sf::Vector2i(r, c);
                return;
            }
        }
    }

    moused = sf::Vector2i(-1, -1);
}

void Inventory_Interface::loadReactions(std::vector<Reaction> reactions, Item_Library& item_library)
{
    reaction_interface.load(reactions, inventory, item_library);
}

std::shared_ptr<Item> Inventory_Interface::mousedItem()
{
    if (moused.x >= 0) {
        return cells[moused.x][moused.y].getItem();
    }
    else {
        return nullptr;
    }
}

Inventory_Cell* Inventory_Interface::mousedCell()
{
    if (moused.x >= 0) {
        return &cells[moused.x][moused.y];
    }

    return nullptr;
}

void Inventory_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (open) {
        target.draw(frame, states);

        target.draw(reaction_interface, states);
        target.setView(view);

        target.draw(progress_bar, states);
        target.draw(progress_arrow, states);
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
