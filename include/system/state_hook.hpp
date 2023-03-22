#pragma once

#include <system/state.hpp>

/// STATE HOOK ///
/// \brief
///
class State_Hook {
protected:

//////////////////////
///// MAIN STATE /////

    static Main_State state_main; /**< as defined in system/state */
    static Main_State prev_main; /**< for main state reversions */
    static bool change_main; /**< flag indicating the state has changed and shell needs to re-read */

/// newMain ///
///
/// \brief changes the main state
///
    static void newMain(Main_State ns);

//////////////////////
///// MENU STATE /////

    static Menu_State state_menu; /**< as defined in system/state */
    static Menu_State prev_menu; /**< for menu state reversions */
    static bool change_menu; /**< flag indicating the menu state has been changed and must be re-read */

/// newMenu ///
///
/// \brief changes the menu state
///
    static void newMenu(Menu_State ns); //new menu
};
