#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <string>
#include <variant>

/// ACTION TRIGGER ///
///
/// \brief alias for a variant holding sf::Keyboard::Key, sf::Mouse::Button, or a string
/// (does this work for using?)
///
using Action_Trigger = std::variant<sf::Keyboard::Key, sf::Mouse::Button, std::string>;
