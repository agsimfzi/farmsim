#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <string>

#include "action_trigger.hpp"

/////////////////////////////////////////////////////////////
/// \brief This class handles conversions between
/// sf::Keyboard::Key/sf::Mouse::Button and std::string
/// it is a bimap-less modification of Bromeon's solution in Thor
/// (with a hacky workaround for mouse buttons)
///
class Convert_Action_Trigger {
public:
    Convert_Action_Trigger();

    std::string toString(const Action_Trigger& trigger);
    Action_Trigger toKey(const std::string& str);

private:
    static std::map<sf::Keyboard::Key, std::string> key_to_string;
    static std::map<std::string, sf::Keyboard::Key> string_to_key;

    static std::map<sf::Mouse::Button, std::string> button_to_string;
    static std::map<std::string, sf::Mouse::Button> string_to_button;

    void init();
};
