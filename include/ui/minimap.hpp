#pragma once

#include <SFML/Graphics.hpp>

#include <world/world.hpp>

class Minimap : public sf::Drawable {
public:
    Minimap();

    void load(World& world);

    void update(sf::Vector2i player_coordinates);

    void expand();
    void close();
    bool isExpanded();

    void startDrag();
    void endDrag();
    void checkDrag();

    void zoom(float delta);

private:
    sf::VertexArray map;
    sf::View view;

    sf::Vector2i player_coordinates;

    bool expanded = false;
    bool dragging = false;

    sf::Vector2i drag_pos;

    sf::CircleShape player_blip;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
