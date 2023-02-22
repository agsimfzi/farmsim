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
    void loadDrop(std::function<void(std::shared_ptr<Item>)> drop);

    virtual void update(sf::RenderWindow& window);

    std::vector<std::vector<Inventory_Cell>> cells;

    void readInventory();
    void writeInventory();

    void setEquippedIndex(size_t active);
    size_t getEquippedIndex();

    bool scroll(float delta, sf::RenderWindow& window);
    void scrollLeft();
    void scrollRight();

    size_t screenWidth = 1920;
    size_t screenHeight = 1080;

    void clickLeft(sf::RenderWindow& window);
    void clickRight();

    virtual void shiftClickLeft () {}
    virtual void shiftClickRight() {}
    // consider implementing these to move between quickbar

    bool open = false;
    bool dragging = false;
    bool expanded = false;

    void close();

    void startDrag();
    void checkDrag();
    void endDrag();
    void cancelDrag();

    std::shared_ptr<Item> dragItem;
    sf::Text dragCountText;

    virtual void loadReactions(std::vector<Reaction> reactions, Item_Library& item_library);

protected:
    Player_Inventory& inventory;

    sf::Vector2i moused;

    sf::View& view;

    Reaction_Interface reaction_interface;

    void updateDragText();

    const static float cell_padding;
    sf::Vector2i dragStartIndex;
    Crafting* crafting = nullptr;
    static size_t equippedIndex;

    sf::RectangleShape frame;

    sf::RectangleShape progress_bar;
    sf::RectangleShape progress_arrow;

    void placeMergeSwap();
    void swap();
    void merge();

    void resize();
    void placeCells();
    void mousedIndex();
    void clearProgressBar();

    void checkTooltip(sf::RenderWindow& window);

    std::shared_ptr<Tooltip> active_tooltip;
    sf::Vector2i tooltip_index{ -1, -1 };
    bool reaction_tooltip = false;

    virtual void readExtension (){}
    virtual void writeExtension(){}

    virtual bool checkReactionInterface(sf::RenderWindow& window);

    static std::function<void(std::shared_ptr<Item>)> drop;

    std::shared_ptr<Item> mousedItem();
    Inventory_Cell* mousedCell();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
