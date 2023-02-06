#include <ui/inventory_interface.hpp>

#include <SFML/Window/Mouse.hpp>

#include <resources/font_manager.hpp>

#include <world/tile.hpp>

#include <util/vector2_stream.hpp>

const float Inventory_Interface::cell_padding = Inventory_Cell::frameOutlineSize;

Inventory_Interface::Inventory_Interface(Player_Inventory& inventory)
    : inventory { inventory }
{
    cells.push_back(std::vector<Inventory_Cell>());
    resize();
    readInventory();

    setEquippedIndex(0);

    dragCountText.setFont(Font_Manager::get(Font::UI));
    dragCountText.setFillColor(sf::Color::Black);

    progress_bar.setFillColor(sf::Color::White);
    progress_bar.setSize(sf::Vector2f(Tile::tileSize, 0.f));
}

void Inventory_Interface::readInventory()
{
    size_t nr = inventory.rowCount;
    size_t nc = inventory.rowWidth;

    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].setItem(inventory(r, c));
        }
    }
}

void Inventory_Interface::update()
{
    checkDrag();

    if (building && building->type != Building::CONTAINER && building->current_reaction >= 0) {
        checkReaction();
        setProgressBarSize();
    }
}

void Inventory_Interface::checkReaction()
{
    cells[inventory.rowCount].front().setItem(building->activeReagant());
    cells[inventory.rowCount + 1].front().setItem(building->activeProduct());
}

void Inventory_Interface::setProgressBarSize()
{
    sf::Vector2f size;
    size.x = Tile::tileSize;
    size.y = (Tile::tileSize * building->progress());
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

    while (cells.size() > inventory.rowCount) {
        cells.pop_back();
    }
}

void Inventory_Interface::startDrag()
{
    const sf::Vector2i mpos = sf::Mouse::getPosition();
    sf::Vector2i index = mousedIndex(mpos);
    if (index.x >= 0 && index.y >= 0) {
        sf::Vector2u i(index);
        Item* item = cells[i.x][i.y].getItem();
        if (item) {
            std::cout << "dragging " << item->count() << ' ' << item->getName() << '\n';
            dragItem = std::make_shared<Item>(*item);
            cells[i.x][i.y].clearItem();
            dragging = true;
            dragStartIndex = index;
            std::string text = "";
            if (dragItem->count() > 1) {
                text = std::to_string(dragItem->count());
            }
            dragCountText.setString(text);
            if (i.x < inventory.rowCount) {
                std::cout << "\terasing from inventory\n";
                inventory.clearItem(i.x, i.y);
            }
            else if (building) {
                std::cout << "\t\treading building\n";
                if (building->type != Building::CONTAINER) {
                    std::cout << "\t\tproper type\n";
                    if (i.x == inventory.rowCount) {
                        std::cout << "\terasing reagant\n";
                        building->clearReagant();
                        clearProgressBar();
                    }
                    else if (i.x == inventory.rowCount + 1) {
                        std::cout << "\terasing product\n";
                        building->clearProduct();
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
        dragItem.reset();
    }
}

void Inventory_Interface::placeMergeSwap(sf::Vector2i i)
{
    sf::Vector2i dsi = dragStartIndex;
    Item* si = cells[i.x][i.y].getItem();
    if (i.x >= (int)inventory.rowCount) { // BUILDING PLACEMENT
        if (building) {
            if (building->type != Building::CONTAINER) {
                if (i.x == (int)inventory.rowCount && building->validReagant(dragItem->getName())) {
                    Item* reagant = building->activeReagant();
                    if (!reagant) {
                        building->setReagant(dragItem.get());
                        cells[i.x][i.y].setItem(dragItem.get());
                        dragItem.reset();
                    }
                    else if (reagant->getUID() == dragItem->getUID()) {
                        building->setReagant(dragItem.get());
                        cells[i.x][i.y].setItem(dragItem.get());
                        dragItem.reset();
                    }
                    else {
                        swap(i);
                        building->checkReaction();
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
                    dragItem.reset();
                }
                else {
                    swap(i);
                }
            }
            else {
                cells[i.x][i.y].setItem(dragItem.get());
                inventory.placeItem(i.x, i.y, dragItem.get());
                dragItem.reset();
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
                            dragItem.reset();
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
                        dragItem.reset();
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
    dragItem.reset();

    if (dsi.x < (int)inventory.rowCount) {
        inventory.placeItem(dsi.x, dsi.y, cells[dsi.x][dsi.y].getItem());
    }
    else if (building->type != Building::CONTAINER) {
        building->clearReagant();
        building->setReagant(cells[dsi.x][dsi.y].getItem());
    }

    if (i.x < (int)inventory.rowCount) {
        inventory.placeItem(i.x, i.y, cells[i.x][i.y].getItem());
    }
    else if (building->type != Building::CONTAINER) {
        building->clearReagant();
        building->setReagant(cells[i.x][i.y].getItem());
    }
}

void Inventory_Interface::cancelDrag()
{
    cells[dragStartIndex.x][dragStartIndex.y].setItem(dragItem.get());
    if (dragStartIndex.x >= (int)inventory.rowCount && building) {
        if (building->type != Building::CONTAINER) {
            if (dragStartIndex.x == (int)inventory.rowCount) {
                building->setReagant(dragItem.get());
                building->checkReaction();
            }
            else if (dragStartIndex.x == (int)inventory.rowCount + 1) {
                building->setProduct(dragItem.get());
            }
        }
    }
    dragging = false;
    dragItem.reset();
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
                std::cout << "found moused index at " << index << '\n';
                goto END;
            }
        }
    }

    END: return index;
}

void Inventory_Interface::loadBuilding(Building* b)
{
    while (cells.size() > inventory.rowCount) {
        cells.pop_back();
    }
    building = b;
    if (b && b->interface) {
        if (b->type != Building::CONTAINER) {
            sf::Vector2f pos = cells[1].back().getPosition();
            pos.x += (Tile::tileSize * 2.f);
            std::vector<Inventory_Cell> c;
            c.push_back(Inventory_Cell(b->activeReagant()));
            c.back().setPosition(pos);
            cells.emplace_back(std::move(c));
            pos.x -= (Tile::tileSize / 2.f);
            pos.y += (Tile::tileSize / 2.f);
            progress_bar.setPosition(pos);
            pos.x += (Tile::tileSize / 2.f);
            pos.y += (Tile::tileSize * 1.5f);
            c.clear();
            c.push_back(Inventory_Cell(b->activeProduct()));
            c.back().setPosition(pos);
            cells.emplace_back(std::move(c));

            std::cout << "loaded building, cell row count is now " << cells.size() << '\n';

            setProgressBarSize();
        }
        else {
            building = nullptr;
            clearProgressBar();
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
            if (r >= inventory.rowCount) {
                //std::cout << "drawing cell " << r << ", " << c << '\n';
            }
        }
    }

    if (open && building && building->current_reaction >= 0) {
        target.draw(progress_bar, states);
    }

    if (dragging && dragItem) {
        target.draw(*dragItem, states);
        target.draw(dragCountText, states);
    }
}
