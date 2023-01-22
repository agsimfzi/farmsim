#pragma once

#include <memory>

#include <item/player_inventory.hpp>

#include "inventory_cell.hpp"

class Inventory_Interface : public sf::Drawable {
public:
    Inventory_Interface(Player_Inventory& inventory);

    Player_Inventory& inventory;

    std::vector<std::vector<Inventory_Cell>> cells  {{ }};

    void readInventory();

    void setEquippedIndex(size_t active);
    size_t getEquippedIndex();

    void scrollLeft();
    void scrollRight();

    size_t screenWidth = 1920;
    size_t screenHeight = 1080;

    void pollChanges();

    bool expanded = false;

    bool dragging = false;

    void close();

    void startDrag();
    void checkDrag();
    void endDrag();

    std::shared_ptr<Item> dragItem;
    sf::Text dragCountText;

private:
    void resize();
    void placeCells();
    size_t equippedIndex = 0;
    sf::Vector2i mousedIndex(const sf::Vector2i& mpos);
    const static float cell_padding;
    sf::Vector2i dragStartIndex;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
