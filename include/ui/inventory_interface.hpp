#pragma once

#include <memory>

#include <item/player_inventory.hpp>

#include <world/container.hpp>
#include <world/machine.hpp>

#include "building_interface.hpp"
#include "inventory_cell.hpp"

class Inventory_Interface : public sf::Drawable {
public:
    Inventory_Interface(Player_Inventory& inventory);

    void update();

    std::vector<std::vector<Inventory_Cell>> cells;

    void readInventory();

    void setEquippedIndex(size_t active);
    size_t getEquippedIndex();

    void scrollLeft();
    void scrollRight();

    size_t screenWidth = 1920;
    size_t screenHeight = 1080;

    void pollChanges();

    bool open = false;
    bool dragging = false;
    bool expanded = false;

    void close();

    void startDrag();
    void checkDrag();
    void endDrag(std::function<void(std::shared_ptr<Item>)> drop);
    void cancelDrag();

    std::shared_ptr<Item> dragItem;
    sf::Text dragCountText;

    void loadBuilding(Building* b);

private:
    Player_Inventory& inventory;

    void placeMergeSwap(sf::Vector2i i);
    void swap(sf::Vector2i i);
    void resize();
    void placeCells();
    size_t equippedIndex = 0;
    sf::Vector2i mousedIndex(const sf::Vector2i& mpos);
    const static float cell_padding;
    sf::Vector2i dragStartIndex;
    Machine* machine = nullptr;
    Container* container = nullptr;
    sf::RectangleShape progress_bar;
    void setProgressBarSize();
    void clearProgressBar();
    void checkReaction();
    void readBuilding();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
