#pragma once

#include "music_player.hpp"
#include "sound_player.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Audio_Settings : private Sound_Player, private Music_Player {
public:
    void setVolumeMusic(float volume);
    float getVolumeMusic();

    void setVolumeGame(float volume);
    float getVolumeGame();

    void setVolumeUI(float volume);
    float getVolumeUI();
};
