#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <string>

#include "mouse_event.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
struct Input_Package {
    Input_Package();

    //keyPressed map contains a name string for the settings menu key mapper
    std::unordered_map<sf::Keyboard::Key, std::pair<std::string, std::function<void()>>> keyPressed;
    std::map<sf::Keyboard::Key, std::function<void()>> keyReleased;
    std::unordered_map<Mouse_Event, std::pair<std::string, std::function<void()>>> mouse;
    std::function<void(float)> scroll;
    std::function<void()> focus_lost;

    std::function<void(sf::Keyboard::Key)> special;

    void clear();
};
