#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <string>

#include "action_trigger.hpp"

/// ACTION ///
///
/// \brief stores a function called by the trigger's input
///
class Action {
public:
    Action() = default;

/// FULL CONSTRUCTOR ///
///
    Action(const std::string& name, const Action_Trigger& trigger,
        std::function<void()> press,
        std::function<void()> release)
        : name { name }
        , trigger { trigger }
        , press { press }
        , release { release }
    {}

    std::string name; /**< string identifier */
    Action_Trigger trigger; /**< input which triggers the action */
    std::function<void()> press; /**< action called on input press */
    std::function<void()> release; /**< action called on input release */
};
