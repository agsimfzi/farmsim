#pragma once

/// MAIN STATE ///
///
/// \brief main program state for high abstraction delegation
/// (note: move season change to game state)
/// (also implement a state machine in shell, performance isnt bad but more margin is always good)
///
enum class Main_State {
    MENU,
    NEWGAME,
    GAME,
    SEASON_CHANGE,
    LOADING,
    QUIT,
    NULL_STATE
};

/// MENU STATE ///
///
/// \brief tracks the menu that is currently active, or that will be activated on opening
///
enum class Menu_State {
    MAIN,
    PAUSE,
    SETTINGS,
    KEYS,
    NULL_STATE
};
