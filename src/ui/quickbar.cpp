#include <ui/quickbar.hpp>

Quickbar::Quickbar(Player_Inventory& inventory)
    : inventory{ inventory }
{
    readInventory();

    setActiveCell(0);
}

void Quickbar::readInventory()
{
    size_t n = cells.size();
    size_t r = inventory.rowWidth;

    if (n != r) {
        resize();
    }

    size_t row = 0;

    for (size_t i = 0; i < n; i++) {
        cells[i].setItem(inventory(row, i));
    }
}

void Quickbar::pollChanges()
{
    if (inventory.changed) {
        readInventory();
        inventory.changed = false;
    }
}

void Quickbar::setActiveCell(size_t active)
{
    if (active >= cells.size()) {
        active = 0;
    }
    else if (active < 0) {
        active = cells.size() - 1;
    }

    cells[activeCell].deactivate();
    activeCell = active;
    cells[activeCell].activate();
}

size_t Quickbar::getActiveIndex()
{
    return activeCell;
}

void Quickbar::scrollLeft()
{
    if (activeCell == 0) {
        setActiveCell(cells.size() - 1);
    }
    else {
        setActiveCell(activeCell - 1);
    }
}

void Quickbar::scrollRight()
{
    setActiveCell(activeCell + 1);
}

void Quickbar::resize()
{
    size_t n = cells.size();
    size_t r = inventory.rowWidth;

    if (n > r) {
        cells.clear();
        n = 0;
    }

    sf::Vector2f pos(96, screenHeight - 96);

    for (size_t i = n; i < r; i++) {
        cells.push_back(Inventory_Cell());
        cells.back().setPosition(pos);
        pos.x += Inventory_Cell::frameSize.x;
    }
}

void Quickbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& cell : cells) {
        target.draw(cell, states);
    }
}
