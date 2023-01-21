#pragma once

#include "sound_bus_reader.hpp"
#include "sound_player.hpp"

/////////////////////////////////////////////////////////////
/// \brief Converts Sound_Game and Sound_UI types into sf::SoundBuffer objects,
/// which are then passed to Sound_Player to be constructed as sf::Sound
///
class Sound_Interface {
public:
    Sound_Interface() = default;

    void update();

protected:
    Sound_Bus_Reader busReader;
    Sound_Player player;
};
