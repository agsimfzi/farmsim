#include <audio/audio_settings.hpp>

//////////////////////////////////////////////////////////////

void Audio_Settings::setVolumeMusic(float volume)
{
    Music_Player::volume = volume;
    Music_Player::volume_tracker = volume;
    Music_Player::music.setVolume(volume);
}

float Audio_Settings::getVolumeMusic()
{
    return Music_Player::volume;
}

void Audio_Settings::setVolumeGame(float volume)
{
    volume_game = volume;
    for (auto& it : sounds_game) {
        it.setVolume(volume_game);
    }
}

float Audio_Settings::getVolumeGame()
{
    return volume_game;
}

void Audio_Settings::setVolumeUI(float volume)
{
    volume_ui = volume;
    for (auto& it : sounds_ui) {
        it.setVolume(volume_ui);
    }
}

float Audio_Settings::getVolumeUI()
{
    return volume_ui;
}
