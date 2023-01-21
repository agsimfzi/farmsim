#pragma once

#include <queue>
#include <string>

#include "sound.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Sound_Bus {
public:
    static void addSound(Sound_Game sound);
    static void addSound(Sound_UI sound);

    Sound_Game nextGameSound();
    Sound_UI nextUISound();

private:
    static std::queue<Sound_Game> next_game_sound;
    static std::queue<Sound_UI> next_ui_sound;
};
