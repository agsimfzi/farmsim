#pragma once

#include <audio/music_interface.hpp>
#include <audio/sound_interface.hpp>

#include <game/game.hpp>
#include <game/loading.hpp>

#include <input/input_handler.hpp>

#include <menu/feedback.hpp>
#include <menu/menu.hpp>

#include <resources/font_manager.hpp>

#include <ui/ui.hpp>

#include <util/primordial.hpp>

/////////////////////////////////////////////////////////////
/// \brief
///
class Shell : public State_Hook {
public:
    Shell();
    void run();

private:
    sf::RenderWindow window;

    Game game { window, viewGame };
    sf::View viewGame;

    UI ui { game };
    sf::View viewUI;

    Menu* menu = nullptr;
    sf::View viewMenu;
    Menu_Main menu_main;
    Menu_Pause menu_pause;
    Menu_Settings menu_settings;
    Menu_Input menu_input;

    //Input_Handler input{ window, game, ui };
    Input_Handler input { window, game, ui, Menu_Package(&menu_main, &menu_pause, &menu_settings, &menu_input) };

    void update();
    void draw();

    Loading_Screen loadingScreen;

    void loadNewLevel();

    void alignState();

    sf::Clock timestepClock;
    float frameTime { 0.f };
    float targetTime { 1.f / 60.f };
    float deltaTime { 0.f };

    sf::Clock fpsClock;
    sf::Text fpsText;

    Sound_Interface sound;
    Music_Interface music;
};
