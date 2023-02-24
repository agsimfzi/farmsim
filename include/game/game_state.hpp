#pragma once

#include <string>

enum Game_State {
    STANDARD,
    SEASON_CHANGE
};

inline std::string gameStateToString(Game_State s)
{
    switch (s) {
        case Game_State::STANDARD:
            return "STANDARD";
        case Game_State::SEASON_CHANGE:
            return "SEASON_CHANGE";
        default:
            return "INEXPLICABLE GAME_STATE NULL VALUE";
    }
}
