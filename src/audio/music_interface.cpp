#include <audio/music_interface.hpp>

Music_Interface::Music_Interface()
{
    player.setSong(getFilename("MENU"));
}

void Music_Interface::update(Main_State state_main, Menu_State state_menu, Game_State state_game)
{
    if (state_main != this->state_main) {
        if (state_main == Main_State::MENU && state_menu == Menu_State::MAIN) {
            player.setSong(getFilename("MENU"));
        }
        else if (state_main == Main_State::GAME) {
            processGameState(state_game);
        }
        else if (state_main == Main_State::LOADING) {
            player.setSong(getFilename("LOADING"));
        }
    }
    else if (state_game != this->state_game) {
        processGameState(state_game);
    }

    this->state_main = state_main;
    this->state_menu = state_menu;
    this->state_game = state_game;

    player.update();
}

void Music_Interface::processGameState(Game_State state_game)
{
    std::string state = gameStateToString(state_game);
    player.setSong(getFilename(state));
}

std::string Music_Interface::getFilename(std::string baseName)
{
    return std::string("soundtrack/" + baseName + ".wav");
}
