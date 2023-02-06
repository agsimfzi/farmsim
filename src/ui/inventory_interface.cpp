#include <ui/inventory_interface.hpp>

#include <SFML/Window/Mouse.hpp>

#include <resources/font_manager.hpp>

#include <iostream>

const float Inventory_Interface::cell_padding = Inventory_Cell::frameOutlineSize;

Inventory_Interface::Inventory_Interface(Player_Inventory& inventory)
    : inventory { inventory }
{
    readInventory();

    setEquippedIndex(0);

    dragCountText.setFont(Font_Manager::get(Font::UI));
    dragCountText.setFillColor(sf::Color::Black);
}

void Inventory_Interface::readInventory()
{
    size_t nr = cells.size();
    size_t nc = cells[0].size();

    if (nr != inventory.rowCount
     || nc != inventory.rowWidth) {
        resize();
    }

    if (nc != inventory.rowWidth);

    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].setItem(inventory(r, c));
        }
    }
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
    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].setPosition(pos);
            pos.x += Inventory_Cell::frameSize.x + cell_padding;
        }
        pos.x = x;
        if (r == 0) {
            pos.y -= Inventory_Cell::frameSize.y * 5.f;
        }
        else {
            pos.y += Inventory_Cell::frameSize.y + cell_padding;
        }
    }
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
}

size_t Inventory_Interface::getEquippedIndex()
{
    return equippedIndex;
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
    building = nullptr;
}

void Inventory_Interface::startDrag()
{
    const sf::Vector2i mpos = sf::Mouse::getPosition();
    sf::Vector2i index = mousedIndex(mpos);
    if (index.x >= 0 && index.y >= 0) {
        sf::Vector2u i(index);
        Item* item = cells[i.x][i.y].getItem();
        if (item) {
            dragItem = std::make_shared<Item>(*item);
            if (dragItem) {
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
                else if (building) {
                    if (building->type != Building::CONTAINER) {
                        if (i.x == cells.size() - 2) {
                            building->active_reagant = nullptr;
                        }
                        else if (i.x == cells.size() - 1) {
                            building->active_product = nullptr;
                        }
                    }
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

void Inventory_Interface::endDrag(std::function<void(Item*)> drop)
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
        drop(dragItem.get());
        dragItem = nullptr;
    }
}

void Inventory_Interface::placeMergeSwap(sf::Vector2i i)
{
    sf::Vector2i dsi = dragStartIndex;
    Item* si = cells[i.x][i.y].getItem();
    if (i.x >= (int)inventory.rowCount) { // BUILDING PLACEMENT
        if (building) {
            if (building->type != Building::CONTAINER) {
                if (i.x < (int)cells.size() - 1 && building->validReagant(dragItem->getName())) {
                    Item* reagant = building->active_reagant.get();
                    if (!reagant) {
                        reagant = dragItem.get();
                        building->checkReaction();
                        cells[i.x][i.y].setItem(reagant);
                        dragItem = nullptr;
                    }
                    else if (reagant->getUID() == dragItem->getUID()) {
                        reagant->add(dragItem->count());
                        cells[i.x][i.y].setItem(reagant);
                        dragItem = nullptr;
                    }
                    else {
                        swap(i);
                    }
                }
                else {
                    dragging = true;
                    return;
                }
            }
        }
    } // END BUILDING PLACEMENT
    else { // INVENTORY PLACEMENT
        if (dsi.x < (int)inventory.rowCount) {
            if (si) {
                if (si->getUID() == dragItem->getUID()) {
                    si->add(dragItem->count());
                    cells[i.x][i.y].setItem(si);
                    inventory.placeItem(i.x, i.y, si);
                    dragItem = nullptr;
                }
                else {
                    swap(i);
                }
            }
            else {
                cells[i.x][i.y].setItem(dragItem.get());
                cells[i.x][i.y].setItem(dragItem.get());
                inventory.placeItem(i.x, i.y, dragItem.get());
                dragItem = nullptr;
            }
        }
        else if (building) {
            if (building->type != Building::CONTAINER) {
                if (i.x == (int)cells.size() - 1) {
                    dragging = true;
                }
                else {
                    if (si) {
                        if (si->getUID() == dragItem->getUID()) {
                            si->add(dragItem->count());
                            cells[i.x][i.y].setItem(si);
                            inventory.placeItem(i.x, i.y, si);
                            dragItem = nullptr;
                        }
                        if (building->validReagant(si->getName())) {
                            swap(i);
                        }
                        else {
                            dragging = true;
                        }
                    }
                    else {
                        cells[i.x][i.y].setItem(dragItem.get());
                        inventory.placeItem(i.x, i.y, dragItem.get());
                        dragItem = nullptr;
                    }
                }
            }
        }
    } // END INVENTORY PLACEMENT
}

void Inventory_Interface::swap(sf::Vector2i i)
{
    sf::Vector2i dsi = dragStartIndex;
    cells[dsi.x][dsi.y].setItem(cells[i.x][i.y].getItem());
    cells[i.x][i.y].clearItem();
    cells[i.x][i.y].setItem(dragItem.get());
    dragItem = nullptr;

    if (dsi.x < (int)inventory.rowCount) {
        inventory.placeItem(dsi.x, dsi.y, cells[dsi.x][dsi.y].getItem());
    }
    else if (building->type != Building::CONTAINER) {
        building->active_reagant = nullptr;
        building->active_reagant = std::make_shared<Item>(*cells[dsi.x][dsi.y].getItem());
    }

    if (i.x < (int)inventory.rowCount) {
        inventory.placeItem(i.x, i.y, cells[i.x][i.y].getItem());
    }
    else if (building->type != Building::CONTAINER) {
        building->active_reagant = nullptr;
        building->active_reagant = std::make_shared<Item>(*cells[i.x][i.y].getItem());
    }
}

void Inventory_Interface::cancelDrag()
{
    cells[dragStartIndex.x][dragStartIndex.y].setItem(dragItem.get());
    dragging = false;
    dragItem = nullptr;
}

sf::Vector2i Inventory_Interface::mousedIndex(const sf::Vector2i& mpos)
{
    sf::Vector2i index(-1, -1);

    size_t nr = cells.size();
    size_t nc;

    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();
        for (size_t c = 0; c < nc; c++) {
            if (cells[r][c].contains(mpos)) {
                index = sf::Vector2i(r, c);
                goto END;
            }
        }
    }

    END: return index;
}

void Inventory_Interface::loadBuilding(Building* b)
{
    building = b;
    if (b && b->interface) {
        if (b->type != Building::CONTAINER) {
            sf::Vector2f pos(1400.f, 400.f);
            cells.push_back(std::vector<Inventory_Cell>());
            cells.back().push_back(Inventory_Cell(b->active_reagant.get()));
            cells.back().back().setPosition(pos);
            pos.y += 200.f;
            cells.push_back(std::vector<Inventory_Cell>());
            cells.back().push_back(Inventory_Cell(b->active_product.get()));
            cells.back().back().setPosition(pos);
        }
    }
}

void Inventory_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    size_t nr = cells.size();
    size_t nc;
    for (size_t r = 0; r < nr; r++) {
        nc = cells[r].size();
        for (size_t c = 0; (c < nc) && (open || r == 0); c++) {
            target.draw(cells[r][c], states);
        }
    }

    if (dragging && dragItem) {
        target.draw(*dragItem, states);
        target.draw(dragCountText, states);
    }
}
