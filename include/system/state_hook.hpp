#pragma once

#include <system/state.hpp>

/////////////////////////////////////////////////////////////
/// \brief
///
class State_Hook {
protected:
    static Main_State state_main; //main state
    static Main_State prev_main; //previous main
    static bool change_main; //main change
    static void newMain(Main_State ns); //new main

    //////////////////////////////////////////////////////////////////////////////

    static Menu_State state_menu; //menu state
    static Menu_State prev_menu; //previous menu
    static bool change_menu; //menu change
    static void newMenu(Menu_State ns); //new menu
};
