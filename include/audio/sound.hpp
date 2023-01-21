#pragma once

#include <string>

/////////////////////////////////////////////////////////////
/// \brief
///
enum class Sound_Game {
    FOOTSTEP,
    SHOOT,
    SNIPE,
    NULL_SOUND
};

enum class Sound_UI {
    TRIGGER_BUTTON,
    MOVE_SLIDER,
    NULL_SOUND
};

inline Sound_Game stringToGameSound(std::string s)
{
    if (s == "FOOTSTEP")
        return Sound_Game::FOOTSTEP;
    else if (s == "SHOOT")
        return Sound_Game::SHOOT;
    else if (s == "SNIPE")
        return Sound_Game::SNIPE;
    else
        return Sound_Game::NULL_SOUND;
}

inline Sound_UI stringToUISound(std::string s)
{
    if (s == "TRIGGER_BUTTON") {
        return Sound_UI::TRIGGER_BUTTON;
    }
    else if (s == "MOVE_SLIDER") {
        return Sound_UI::MOVE_SLIDER;
    }
    else {
        return Sound_UI::NULL_SOUND;
    }
}
