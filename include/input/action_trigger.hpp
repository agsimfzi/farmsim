#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <string>
#include <variant>

////////////////////////////////////////////
/// \brief does this work for this?
///
using Action_Trigger = std::variant<sf::Keyboard::Key, sf::Mouse::Button, std::string>;
