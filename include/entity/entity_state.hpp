#pragma once

#include <string>

/////////////////////////////////////////////////////////////
/// \brief
///
enum class Entity_State {
    IDLE,
    MOVING,
    NULL_STATE
};

inline std::string entityStateToString(Entity_State s)
{
    switch (s) {
        case Entity_State::IDLE:
            return "IDLE";
        case Entity_State::MOVING:
            return "MOVING";
        default:
            return std::string();
    }
}
