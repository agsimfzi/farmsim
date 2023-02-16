#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include <item/item.hpp>
#include <item/reaction.hpp>

class Tooltip : public sf::Drawable {
public:
    Tooltip() = default;

    /////////////////////////////////////////////////////////////
    /// \brief Constructs an inventory cell tooltip from that cell's item
    ///
    Tooltip(std::shared_ptr<Item> item);

    Tooltip(Reaction& reaction, std::vector<std::shared_ptr<Item>> reagants);

    void setPosition(sf::Vector2f pos);

private:
    sf::RectangleShape frame;

    sf::Text name;
    sf::Text description;
    sf::Text value;

    std::vector<std::pair<sf::Sprite, sf::Text>> labeled_sprites;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
