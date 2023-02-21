#pragma once

#include <string>

/////////////////////////////////////////////////////////////
/// \brief
///
enum class Entity_State {
    IDLE,
    MOVING,
    BOATING,
    BROOMING,
    NULL_STATE
};

inline std::string entityStateToString(Entity_State s)
{
    switch (s) {
        case Entity_State::IDLE:
            return "IDLE";
        case Entity_State::MOVING:
            return "MOVING";
        case Entity_State::BOATING:
            return "BOATING";
        case Entity_State::BROOMING:
            return "BROOMING";
        default:
            return std::string();
    }
}
