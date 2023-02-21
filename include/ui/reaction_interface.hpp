#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <item/item_library.hpp>
#include <item/player_inventory.hpp>
#include <item/reaction.hpp>

#include "scrollable.hpp"
#include "tooltip.hpp"

class Reaction_Panel : public sf::Drawable {
public:
    Reaction_Panel() = delete;
    Reaction_Panel(Reaction& rxn, Item_Library& item_library, sf::Vector2f pos, float size_x);

    ~Reaction_Panel();

    bool isAvailable();
    void setAvailable();
    void unsetAvailable();

    bool contains(sf::Vector2f mpos);

    std::shared_ptr<Item> getProduct();

    std::shared_ptr<Tooltip> tooltip;

private:
    bool available;

    sf::Text name;

    std::vector<std::shared_ptr<Item>> reagants;
    std::shared_ptr<Item> product;

    sf::RectangleShape frame;

    const static sf::Color color_bg_available;
    const static sf::Color color_bg_unavailable;
    const static float outline_thickness;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Reaction_Interface : public sf::Drawable, public Scrollable {
public:
    Reaction_Interface() = default;

    void load(std::vector<Reaction> reactions
            , Player_Inventory& inventory
            , Item_Library& item_library);

    void check(Player_Inventory& inventory);

    void close();

    std::pair<Reaction*, std::shared_ptr<Item>> click(sf::Vector2f mpos);

    std::shared_ptr<Tooltip> findTooltip(sf::Vector2f mpos);

private:
    std::vector<Reaction> reactions;
    std::vector<Reaction_Panel> panels;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
