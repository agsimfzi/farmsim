#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <string>

#include "mouse_event.hpp"

/// INPUT PACKAGE ///
///
/// \brief Contains a complete set of input targets
///
struct Input_Package {

/// DEFAULT CONSTRUCTOR ///
///
    Input_Package()
    {
        clear();
    }

    std::unordered_map<sf::Keyboard::Key, std::pair<std::string, std::function<void()>>> keyPressed; /**< key pressed targets. string is included for settings menu input mapper */
    std::map<sf::Keyboard::Key, std::function<void()>> keyReleased; /**< key release targets */
    std::unordered_map<Mouse_Event, std::pair<std::string, std::function<void()>>> mouse; /**< mouse event targets */
    std::function<void(float)> scroll; /**< mousewheel scroll target */
    std::function<void()> focus_lost; /**< triggers when window focus is lost */

    std::function<void(sf::Keyboard::Key)> special; /**< for input mapper */

/// clear ///
///
/// \brief empties the maps and initializes lambdas
///
    void clear()
    {
        keyPressed.clear();
        keyReleased.clear();
        mouse.clear();

        scroll = [](float) { return; };
        focus_lost = [](){};
    }
};
