#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include <map>

#include "sound_bus.hpp"
#include "sound_player.hpp"
#include "sound.hpp"
#include "sound_context.hpp"

/////////////////////////////////////////////////////////////
/// \brief Converts Sound_Game and Sound_UI types into sf::SoundBuffer objects,
/// which are then passed to Sound_Player to be constructed as sf::Sound
///
class Sound_Bus_Reader : private Sound_Player {
public:
    Sound_Bus_Reader();

    void read(Sound_Player& player);

private:
    Sound_Bus bus;
    std::map<Sound_Game, Sound_Context> game_sounds;
    std::map<Sound_UI, Sound_Context> ui_sounds;
};
