#include <audio/sound_bus_reader.hpp>

#include <system/database.hpp>

//////////////////////////////////////////////////////////////

Sound_Bus_Reader::Sound_Bus_Reader()
{
    Database::loadSounds(game_sounds, ui_sounds);
}

void Sound_Bus_Reader::read(Sound_Player& player)
{
    Sound_Game gsound;
    while ((gsound = bus.nextGameSound()) != Sound_Game::NULL_SOUND
        && game_sounds[gsound].ready()) {
        player.addGameSound(game_sounds[gsound].buffer);
    }

    Sound_UI uisound;
    while ((uisound = bus.nextUISound()) != Sound_UI::NULL_SOUND
        && ui_sounds[uisound].ready()) {
        player.addUISound(ui_sounds[uisound].buffer);
    }
}
