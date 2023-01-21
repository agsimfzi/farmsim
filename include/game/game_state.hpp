#pragma once

#include <string>

enum Game_State {
    PEACE,
    COMBAT,
    BOSS,
    WIN,
    LOSS,
};

inline std::string gameStateToString(Game_State s)
{
    switch (s) {
        case Game_State::PEACE:
            return "PEACE";
        case Game_State::COMBAT:
            return "COMBAT";
        case Game_State::BOSS:
            return "BOSS";
        case Game_State::WIN:
            return "WIN";
        case Game_State::LOSS:
            return "LOSS";
        default:
            return "INEXPLICABLE NULL VALUE";
    }
}
