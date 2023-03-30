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

    sf::Vector2f getPosition();
    sf::Vector2f getSize();

private:
    sf::Sprite map;
    sf::Texture texture;

    float zoom_level = 1.f;

    sf::View view;

    sf::Vector2f mini_size{ 300.f, 300.f };
    sf::Vector2f full_size{ 1500.f, 900.f };

    sf::Vector2f screen_size{ 1920.f, 1080.f };

    sf::Vector2f mini_factor;
    sf::Vector2f full_factor;

    float frame_border_size = 2.f;

    sf::Vector2i player_coordinates;

    bool expanded = false;
    bool dragging = false;

    sf::Vector2i drag_pos;

    sf::RectangleShape mini_frame;
    sf::RectangleShape full_frame;
    sf::RectangleShape* active_frame;

    sf::CircleShape player_blip;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
