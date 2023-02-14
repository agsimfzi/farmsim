#pragma once

#include <SFML/Graphics.hpp>

class Scrollable {
public:
    Scrollable() = default;

    void setScrollable(sf::Vector2f pos, sf::Vector2f size, sf::Vector2u window_size, float max_height);

    void scroll(float delta);

    sf::View& getView();

protected:
    sf::RectangleShape scrollbar;
    sf::View view;

    float current_scroll = 0.f;
    float max_scroll;
};
