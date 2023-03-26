#pragma once

#include <SFML/Graphics.hpp>

#include <world/tile.hpp>

/// FLOOR MAP ///
///
/// \brief VertexArray shell for chunk rendering
///
class Floor_Map : public sf::Drawable, public sf::Transformable {
public:
    Floor_Map();

    void load(Map_Tile<Tile>& floor, sf::Vector2i start, sf::Vector2i size);

    void change(size_t index, sf::Vector2i texture_pos, sf::Vector2i size);

    sf::VertexArray vertices;

private:
    sf::Texture* texture;

    sf::Vector2u tsize;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
