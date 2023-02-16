#pragma once

#include <memory>

#include <item/player_inventory.hpp>

#include <world/container.hpp>
#include <world/crafting.hpp>
#include <world/machine.hpp>

#include "inventory_cell.hpp"
#include "reaction_interface.hpp"
#include "tooltip.hpp"

class Inventory_Interface : public sf::Drawable {
public:
    Inventory_Interface(Player_Inventory& inventory, sf::View& view);

    void update(sf::RenderWindow& window);

    std::vector<std::vector<Inventory_Cell>> cells;

    void readInventory();

    void setEquippedIndex(size_t active);
    size_t getEquippedIndex();

    bool scroll(float delta, sf::RenderWindow& window);
    void scrollLeft();
    void scrollRight();

    size_t screenWidth = 1920;
    size_t screenHeight = 1080;

    void pollChanges();

    void clickLeft(sf::RenderWindow& window);
    void clickRight();

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

    void loadBuilding(Building* b, Item_Library& item_library);

private:
    Player_Inventory& inventory;

    sf::View& view;

    Reaction_Interface reaction_interface;

    const static float cell_padding;
    sf::Vector2i dragStartIndex;
    Machine* machine = nullptr;
    Container* container = nullptr;
    Crafting* crafting = nullptr;
    size_t equippedIndex = 0;

    sf::RectangleShape progress_bar;

    void placeMergeSwap(sf::Vector2i i);
    void swap(sf::Vector2i i);
    void resize();
    void placeCells();
    sf::Vector2i mousedIndex(const sf::Vector2i& mpos);
    void setProgressBarSize();
    void clearProgressBar();
    void checkReaction();
    void readBuilding();

    void checkTooltip(sf::RenderWindow& window);

    std::shared_ptr<Tooltip> active_tooltip;
    sf::Vector2i tooltip_index;
    bool reaction_tooltip = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
