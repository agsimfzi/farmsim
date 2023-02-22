#pragma once

#include <SFML/Window/Keyboard.hpp>

inline bool shiftPressed()
{
    return (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
}
