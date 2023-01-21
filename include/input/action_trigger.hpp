#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <string>
#include <variant>

using Action_Trigger = std::variant<sf::Keyboard::Key, sf::Mouse::Button, std::string>;
