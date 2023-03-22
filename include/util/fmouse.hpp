#pragma once

#include <SFML/Window/Mouse.hpp>

/// fMouse ///
///
/// \brief returns the mouse position as Vector2f
///
inline sf::Vector2f fMouse()
{
    return sf::Vector2f(sf::Mouse::getPosition());
}

/// fMouse ///
///
/// \brief returns mouse position as Vector2f, in the context of the passed viewport
///
inline sf::Vector2f fMouse(sf::RenderWindow& window, sf::View view)
{
    return window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
}
