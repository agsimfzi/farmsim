#pragma once

#include <memory>

#include <item/player_inventory.hpp>

#include "inventory_cell.hpp"

class Quickbar : public sf::Drawable{
public:
    Quickbar(Player_Inventory& inventory);

    Player_Inventory& inventory;

    std::vector<Inventory_Cell> cells;

    void readInventory();

    void setActiveCell(size_t active);
    size_t getActiveIndex();

    void scrollLeft();
    void scrollRight();

    size_t screenWidth = 1920;
    size_t screenHeight = 1080;

    void pollChanges();

private:
    void resize();
    size_t activeCell = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
