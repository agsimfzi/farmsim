#pragma once

#include <string>

enum Game_State {
    STANDARD
};

inline std::string gameStateToString(Game_State s)
{
    switch (s) {
        case Game_State::STANDARD:
            return "STANDARD";
        default:
            return "INEXPLICABLE GAME_STATE NULL VALUE";
    }
}
