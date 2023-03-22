#pragma once

#include <string>

/// GAME STATE ///
///
/// \brief currently unimplemented
///
enum Game_State {
    STANDARD
};

/// gameStateToString ///
///
/// \brief converts the passed Game_State to a string
///
inline std::string gameStateToString(Game_State s)
{
    switch (s) {
        case Game_State::STANDARD:
            return "STANDARD";
        default:
            return "INEXPLICABLE GAME_STATE NULL VALUE";
    }
}
