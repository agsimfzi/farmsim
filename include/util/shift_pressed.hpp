#pragma once

#include <SFML/Window/Keyboard.hpp>

/// shiftPressed ///
///
/// \brief returns true if either shift key is pressed
///
inline bool shiftPressed()
{
    return (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
}
