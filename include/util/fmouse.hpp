#pragma once

#include <SFML/Window/Mouse.hpp>

/////////////////////////////////////////////////////////////

inline sf::Vector2f fMouse()
{
    return sf::Vector2f(sf::Mouse::getPosition());
}

inline sf::Vector2f fMouse(sf::RenderWindow& window, sf::View& view)
{
    return window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
}
