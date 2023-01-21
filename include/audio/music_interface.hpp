#pragma once

#include <game/game_state.hpp>
#include <system/state.hpp>

#include "sound.hpp"
#include "music_player.hpp"

class Music_Interface {
public:
    Music_Interface();

    void update(Main_State state_main, Menu_State state_menu, Game_State state_game);

private:
    Music_Player player;

    Main_State state_main;
    Menu_State state_menu;
    Game_State state_game;

    void processGameState(Game_State state_game);
    std::string getFilename(std::string baseName);
};
