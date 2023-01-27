#pragma once

#include <SFML/Graphics.hpp>

#include <world/world.hpp>

class Minimap : public sf::Drawable {
public:
    Minimap();

    void load(Map_Tile<Floor>& floor);

    void update(sf::Vector2i player_coordinates);

private:
    sf::VertexArray map;
    sf::View view;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
