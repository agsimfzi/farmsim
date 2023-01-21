#include <system/state_hook.hpp>

//////////////////////////////////////////////////////////////

Main_State State_Hook::state_main = Main_State::MENU;
Menu_State State_Hook::state_menu = Menu_State::MAIN;
bool State_Hook::change_main = true;
bool State_Hook::change_menu = true;
Main_State State_Hook::prev_main = Main_State::MENU;
Menu_State State_Hook::prev_menu = Menu_State::MAIN;

void State_Hook::newMain(Main_State ns)
{
    if (state_main != ns) {
        prev_main = state_main;
        state_main = ns;
        change_main = true;
    }
}

void State_Hook::newMenu(Menu_State ns)
{
    if (state_menu != ns) {
        prev_menu = state_menu;
        state_menu = ns;
        change_menu = true;
    }
}
