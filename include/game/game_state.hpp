#pragma once

#include <string>

/// GAME STATE ///
///
/// \brief currently unimplemented
///
enum class Game_State {
    PLAY,
    SEASON_CHANGE,
    FADE_IN,
    FADE_OUT
};

/// gameStateToString ///
///
/// \brief converts the passed Game_State to a string
///
inline std::string gameStateToString(Game_State s)
{
    switch (s) {
        case Game_State::PLAY:
            return "PLAY";
        case Game_State::SEASON_CHANGE:
            return "SEASON_CHANGE";
        case Game_State::FADE_IN:
            return "FADE_IN";
        case Game_State::FADE_OUT:
            return "FADE_OUT";
        default:
            return "INEXPLICABLE GAME_STATE NULL VALUE";
    }
}
