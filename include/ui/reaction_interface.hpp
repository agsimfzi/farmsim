#pragma once

#include <SFML/Graphics.hpp>

#include <item/item_library.hpp>
#include <item/player_inventory.hpp>
#include <item/reaction.hpp>

#include "scrollable.hpp"

class Reaction_Panel : public sf::Drawable {
public:
    Reaction_Panel() = default;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Reaction_Interface : public sf::Drawable, public Scrollable {
public:
    Reaction_Interface() = default;

    void load(std::vector<Reaction> reactions, Player_Inventory& inventory);
    void close();

    Reaction* click(sf::Vector2f mpos);

    void checkInventory(Player_Inventory& inventory);

private:
    std::vector<Reaction> reactions;
    std::vector<Reaction_Panel> panels;

    sf::View view;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
