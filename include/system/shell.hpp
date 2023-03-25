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

#include "season_changer.hpp"

/// SHELL ///
///
/// \brief performs and reads changes to main state logic
///
class Shell : private State_Hook {
public:

/// DEFAULT CONSTRUCTOR ///
///
/// \brief opens the render window, and prepares views and input
///
    Shell();

/// run ///
///
/// \brief runs primary game loop while the window is open
///
    void run();

private:
    sf::RenderWindow window; /**< main game window */

    sf::View view_game; /**< game viewport */
    Game game { view_game }; /**< outer game class */

    sf::View view_ui; /**< ui viewport (matches window) */
    UI ui { window, game, view_ui }; /**< outer ui class */

    Season_Changer season_changer{ game, ui }; /**< move this to Game somehow */

    Menu* menu = nullptr; /**< base Menu pointer for polymorphic behavior */
    sf::View view_menu; /**< menu viewport (matches window) */
    Menu_Main menu_main; /**< primary menu, shown at startup */
    Menu_Pause menu_pause; /**< top level menu shown during game */
    Menu_Settings menu_settings; /**< secondary menu for modifying game settings */
    Menu_Input menu_input; /**< tertiary menu for modifying game input */

    Input_Handler input { window, game, ui, Menu_Package(&menu_main, &menu_pause, &menu_settings, &menu_input), season_changer };
        /**< reads mouse and keyboard input and delegates based on states */

/// update ///
///
/// \brief delegates to lower-level update functions based on state
///
    void update();

/// draw ///
///
/// \brief draws encapsulated classes to the render window based on state
    void draw();

    Loading_Screen loading_screen; /**< performs game loading operations while allowing unrelated rendering */

/// loadNewLevel ///
///
/// \brief transitions the state to LOADING and prepares the loading screen
///
    void loadNewLevel();

/// alignState ///
///
/// \brief reads and implements state changes
///
    void alignState();

    sf::Clock timestep_clock; /**< records frame time */
    float frame_time { 0.f }; /**< stores frame time */
    float target_time { 1.f / 60.f }; /**< for calculating delta time */
    float delta_time { 0.f }; /**< frame time as a factor of target time */

    sf::Text fps_text; /**< displays frames per second */

    Sound_Interface sound; /**< to deprecate */
    Music_Interface music; /**< to deprecate */
};
