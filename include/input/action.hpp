#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <string>

#include "action_trigger.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Action {
public:
    Action() { }
    Action(const std::string& name, const Action_Trigger& trigger,
        std::function<void()> press,
        std::function<void()> release)
        : name { name }
        , trigger { trigger }
        , press { press }
        , release { release }
    {
    }

    std::string name;
    Action_Trigger trigger;
    std::function<void()> press;
    std::function<void()> release;
};
