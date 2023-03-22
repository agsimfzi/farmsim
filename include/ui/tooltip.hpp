#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include <item/item.hpp>
#include <item/reaction.hpp>

/// TOOLTIP ///
///
/// \brief item and reaction info panel that appears on mouseover when the inventory interface is open
///
class Tooltip : public sf::Drawable {
public:
    Tooltip() = default;

/// ITEM CONSTRUCTOR ///
///
/// \brief Constructs an item tooltip for an inventory cell
///
    Tooltip(std::shared_ptr<Item> item);

/// REACTION CONSTRUCTOR ///
///
/// \brief Constructs a reaction tooltip for a reaction panel
    Tooltip(Reaction& reaction, std::vector<std::shared_ptr<Item>> reagants);

/// setPosition ///
///
/// \brief places the tooltip. "locks" it against window boundaries so the whole tooltip is always visible.
    void setPosition(sf::Vector2f pos, sf::Vector2f window_size);

/// move ///
///
/// \brief ... moves the tooltip. unsure if this is in use, as positioning is always locked to the mouse
///
    void move(sf::Vector2f offset);

private:
    sf::RectangleShape frame;

    sf::Text name;
    sf::Text description;
    sf::Text value; /**< for displaying monetary value in item tooltips */

    std::vector<std::pair<sf::Sprite, sf::Text>> labeled_sprites; /**< for displaying reagants and products in reaction tooltips */

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
