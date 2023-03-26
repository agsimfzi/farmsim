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

/// FULL CONSTRUCTOR ///
///
/// \brief calls init() on first instancing
///
    Convert_Action_Trigger();

/// toString ///
///
/// \brief returns an action trigger variant, converted to a string
    std::string toString(const Action_Trigger& trigger);

/// toKey ///
///
/// \brief returns an action trigger variant, with a key or a button, converted from a string
    Action_Trigger toKey(const std::string& str);

private:
    static std::map<sf::Keyboard::Key, std::string> key_to_string; /**< conversion map, sf::Keyboard::Key to string */
    static std::map<std::string, sf::Keyboard::Key> string_to_key; /**< conversion map, string to sf::Keyboard::Key */

    static std::map<sf::Mouse::Button, std::string> button_to_string; /**< conversion map, sf::Mouse::Button to string */
    static std::map<std::string, sf::Mouse::Button> string_to_button; /**< conversion map, string to sf::Mouse::Button */

/// init ///
///
/// \brief loads string conversion maps
///
    void init();
};
