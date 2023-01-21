#include <input/input_handler.hpp>

#include <system/database.hpp>

//////////////////////////////////////////////////////////////

Input_Package::Input_Package()
{
    clear();
}

void Input_Package::clear()
{
    keyPressed.clear();
    keyReleased.clear();
    mouse.clear();

    scroll = std::function<void(float)>([](float) { return; });
}

Input_Handler::Input_Handler(sf::RenderWindow& nwindow, Game& game, UI& ui, Menu_Package menu_package)
    : window { nwindow }
{
    Player* player = &game.getPlayer();

    /////////////////////////////////////////////////////////////
    //GAME AND UI INPUTS
    //
    Input_Package& p_g = context[Main_State::GAME];

    p_g.keyReleased[sf::Keyboard::Escape] = std::bind(&Game::escape, &game);

    std::map<std::string, Action_Trigger> actions = Database::getActions();

    for (const auto& action : actions) {
        std::function<void()> press = []() {};
        std::function<void()> release = []() {};

        if (action.first == "Move North") {
            press = std::bind(&Player::upStart, player);
            release = std::bind(&Player::upEnd, player);
        }
        else if (action.first == "Move South") {
            press = std::bind(&Player::downStart, player);
            release = std::bind(&Player::downEnd, player);
        }
        else if (action.first == "Move West") {
            press = std::bind(&Player::leftStart, player);
            release = std::bind(&Player::leftEnd, player);
        }
        else if (action.first == "Move East") {
            press = std::bind(&Player::rightStart, player);
            release = std::bind(&Player::rightEnd, player);
        }

        placeActionTrigger(action, press, release);
    }

    sf::Keyboard::Key key = sf::Keyboard::Num1;

    for (unsigned int i = 0; i < 9; i++) {
        p_g.keyReleased[key] = std::bind(&UI::numRelease, &ui, i);
        key = static_cast<sf::Keyboard::Key>(static_cast<int>(key) + 1);
    }
    p_g.keyReleased[sf::Keyboard::Num0] = [&]() { ui.numRelease(9); };

    p_g.scroll = std::bind(&UI::scroll, &ui, std::placeholders::_1);

    p_g.focus_lost = std::bind(&Game::stopInput, &game);

    p_g.mouse[Mouse_Event::LEFT_CLICK]
        = std::make_pair("use equipped item"
                       , std::bind(&Game::clickLeft, &game));

    p_g.mouse[Mouse_Event::LEFT_RELEASE]
        = std::make_pair("null"
                       , std::bind(&Game::releaseLeft, &game));

    p_g.mouse[Mouse_Event::RIGHT_CLICK]
        = std::make_pair("interact"
                       , std::bind(&Game::clickRight, &game));

    p_g.mouse[Mouse_Event::RIGHT_RELEASE]
        = std::make_pair("end interact"
                       , std::bind(&Game::releaseRight, &game));

    // some event also should end interaction

    /////////////////////////////////////////////////////////////
    //MENU INPUTS
    //

    std::map<Menu_State, Menu*> menus = { { Menu_State::MAIN, menu_package.m_main },
        { Menu_State::PAUSE, menu_package.m_pause },
        { Menu_State::SETTINGS, menu_package.m_settings },
        { Menu_State::KEYS, menu_package.m_keymap } };

    for (const auto& m : menus) {
        Input_Package& p = context_menu[m.first];
        p.keyReleased[sf::Keyboard::Escape] = std::bind(&Menu::back, m.second);
        p.mouse[Mouse_Event::LEFT_CLICK].second = std::bind(&Menu::clickLeft, m.second);
        p.mouse[Mouse_Event::LEFT_RELEASE].second = std::bind(&Menu::releaseLeft, m.second);
        p.mouse[Mouse_Event::RIGHT_CLICK].second = std::bind(&Menu::clickRight, m.second);
        p.mouse[Mouse_Event::RIGHT_RELEASE].second = std::bind(&Menu::releaseRight, m.second);
        p.scroll = std::bind(&Menu::scroll, m.second, std::placeholders::_1);
        p.focus_lost = std::bind(&Menu::stopInput, m.second);
    }

    context_menu[Menu_State::KEYS].special = std::bind(&Menu_Input::keyPressed, menu_package.m_keymap, std::placeholders::_1);
}

void Input_Handler::placeActionTrigger(const std::pair<std::string, Action_Trigger>& action, std::function<void()> press, std::function<void()> release)
{
    Input_Package& p_g = context[Main_State::GAME];

    if (std::holds_alternative<sf::Keyboard::Key>(action.second)) {
        sf::Keyboard::Key k = std::get<sf::Keyboard::Key>(action.second);
        p_g.keyPressed[k].first = action.first;
        p_g.keyPressed[k].second = press;
        p_g.keyReleased[k] = release;
    }
    else if (std::holds_alternative<sf::Mouse::Button>(action.second)) {
        sf::Mouse::Button b = std::get<sf::Mouse::Button>(action.second);
        Mouse_Event p { Mouse_Event::NULL_EVENT };
        Mouse_Event r { Mouse_Event::NULL_EVENT };
        if (b == sf::Mouse::Left) {
            p = Mouse_Event::LEFT_CLICK;
            r = Mouse_Event::LEFT_RELEASE;
        }
        else if (b == sf::Mouse::Right) {
            p = Mouse_Event::RIGHT_CLICK;
            r = Mouse_Event::RIGHT_RELEASE;
        }
        p_g.mouse[p].first = action.first;
        p_g.mouse[p].second = press;
        p_g.mouse[r].second = release;
    }
}

const std::map<std::string, Action> Input_Handler::getRemappableActions()
{
    Input_Package& p_g = context[Main_State::GAME];

    std::map<std::string, Action> actions;

    for (size_t k = 0; k < sf::Keyboard::KeyCount; k++) {
        sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(k);
        if (p_g.keyPressed.contains(key)) {
            actions[p_g.keyPressed[key].first] = Action(p_g.keyPressed[key].first, key, p_g.keyPressed[key].second, p_g.keyReleased[key]);
        }
    }

    Mouse_Event c = Mouse_Event::LEFT_CLICK;
    Mouse_Event r = Mouse_Event::LEFT_RELEASE;
    sf::Mouse::Button b = sf::Mouse::Left;

    if (p_g.mouse.contains(c)) {
        //actions[p_g.mouse[c].first] = Action(p_g.mouse[c].first, b, p_g.mouse[c].second, p_g.mouse[r].second);
    }

    c = Mouse_Event::RIGHT_CLICK;
    r = Mouse_Event::RIGHT_RELEASE;
    b = sf::Mouse::Right;

    if (p_g.mouse.contains(c)) {
        actions[p_g.mouse[c].first] = Action(p_g.mouse[c].first, b, p_g.mouse[c].second, p_g.mouse[r].second);
    }

    return actions;
}

void Input_Handler::setRemappableActions(const std::vector<Action>& actions)
{
    Input_Package& p_g = context[Main_State::GAME];

    Convert_Action_Trigger converter;

    for (auto k = p_g.keyReleased.begin(); k != p_g.keyReleased.end();) {
        if (k->first != sf::Keyboard::Escape) {
            k = p_g.keyReleased.erase(k);
        }
        else {
            ++k;
        }
    }

    p_g.keyPressed.clear();

    p_g.mouse.erase(Mouse_Event::LEFT_CLICK);
    p_g.mouse.erase(Mouse_Event::LEFT_RELEASE);
    p_g.mouse.erase(Mouse_Event::RIGHT_CLICK);
    p_g.mouse.erase(Mouse_Event::RIGHT_RELEASE);

    for (const auto& action : actions) {
        placeActionTrigger(std::make_pair(action.name, action.trigger), action.press, action.release);
    }
}

void Input_Handler::handle()
{
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else {
            if (state_main == Main_State::LOADING
                || state_main == Main_State::NEWGAME
                || state_main == Main_State::NULL_STATE
                || state_main == Main_State::QUIT) {
                continue;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (state_main == Main_State::MENU && state_menu == Menu_State::KEYS) {
                    context[state_main].special(event.key.code);
                }
                else if (context[state_main].keyPressed.count(event.key.code)) {
                    context[state_main].keyPressed[event.key.code].second();
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (context[state_main].keyReleased.count(event.key.code)) {
                    context[state_main].keyReleased[event.key.code]();
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && context[state_main].mouse.count(Mouse_Event::LEFT_CLICK)) {
                    context[state_main].mouse[Mouse_Event::LEFT_CLICK].second();
                }
                else if (event.mouseButton.button == sf::Mouse::Right && context[state_main].mouse.count(Mouse_Event::RIGHT_CLICK)) {
                    context[state_main].mouse[Mouse_Event::RIGHT_CLICK].second();
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (context[state_main].mouse.count(Mouse_Event::LEFT_RELEASE)) {
                        context[state_main].mouse[Mouse_Event::LEFT_RELEASE].second();
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (context[state_main].mouse.count(Mouse_Event::RIGHT_RELEASE)) {
                        context[state_main].mouse[Mouse_Event::RIGHT_RELEASE].second();
                    }
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                context[state_main].scroll(event.mouseWheelScroll.delta);
            }
            else if (event.type == sf::Event::LostFocus) {
                context[state_main].focus_lost();
            }
        }
    }
}

void Input_Handler::menuChange()
{
    context[Main_State::MENU].clear();
    context[Main_State::MENU] = context_menu[state_menu];
}
