#pragma once

#include <SFML/Graphics.hpp>

class Scrollable {
public:
    Scrollable() = default;

    void setView(sf::Vector2f pos, sf::Vector2f size, sf::Vector2u window_size = sf::Vector2u(1920u, 1080u));

    void setScrollable(float max_height);

    sf::View& getView();

    bool scroll(float delta, sf::Vector2f mpos);
    void resizeScrollbar();
    void placeScrollbar();

protected:
    sf::RectangleShape scrollbar;

    sf::FloatRect frame;

    sf::Vector2f size;

    sf::View view;

    float scroll_x_pos;

    float current_scroll = 0.f;
    float max_scroll;
};
