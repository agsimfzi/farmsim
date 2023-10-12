#include <system/shell.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>

//////////////////////////////////////////////////////////////

Shell::Shell()
    : window { sf::VideoMode::getDesktopMode(), "Fairy Farm", sf::Style::Fullscreen }
{
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Cross);
    window.setMouseCursor(cursor);

    window.setKeyRepeatEnabled(false);

    window.setVerticalSyncEnabled(true);

    view_game = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(window.getSize()));
    view_game.setCenter(sf::Vector2f(0.f, 0.f));

    view_ui.setSize(sf::Vector2f(window.getSize()));
    view_ui.setCenter(sf::Vector2f(window.getSize()) / 2.f);

    view_menu.setSize(sf::Vector2f(window.getSize()));
    view_menu.setCenter(sf::Vector2f(window.getSize()) * 0.5f);

    fps_text.setFont(Font_Manager::get(Font::MENU));
    fps_text.setString("0");
    fps_text.setFillColor(sf::Color::Red);
    fps_text.setPosition(sf::Vector2f(8.f, 8.f));

    game.hideUI = std::bind(&UI::hide, &ui);
    game.showUI = std::bind(&UI::show, &ui);

    ui.scale(window);

    menu_input.saveActions = std::bind(&Input_Handler::setRemappableActions, &input, std::placeholders::_1);

    menu_input.setActions(input.getRemappableActions());

    alignState();
}

void Shell::run()
{
    while (window.isOpen()) {
        input.handle();
        update();
        alignState();
        draw();
    }
}

void Shell::update()
{
    sound.update();
    music.update(state_main, state_menu, game.getState());

    frame_time = timestep_clock.getElapsedTime().asSeconds();
    timestep_clock.restart();
    fps_text.setString(std::to_string((int)(1.f / frame_time)));
    delta_time = frame_time / target_time;

    switch (state_main) {
        case Main_State::MENU:
            menu->update(fMouse(window, view_ui));
            break;
        case Main_State::LOADING:
            if (loading_screen.update()) {
                newMain(Main_State::GAME);
                //state_main = Main_State::GAME;
            }
            break;
        case Main_State::GAME:
            game.update(delta_time);
            ui.update();
            game.prepRenderer();
            break;
        case Main_State::SEASON_CHANGE:
            season_changer.update();
            break;
        default:
            break;
    }
}

void Shell::draw()
{
    window.clear();
    switch (state_main) {
        case Main_State::MENU:
            window.setView(view_ui);
            window.draw(*menu);
            break;
        case Main_State::GAME:
            window.setView(view_game);
            window.draw(game);
            window.setView(view_ui);
            window.draw(fps_text);
            window.draw(ui);
            break;
        case Main_State::LOADING:
            window.setView(view_ui);
            window.draw(loading_screen);
            break;
        case Main_State::SEASON_CHANGE:
            window.setView(view_ui);
            window.draw(season_changer);
        default:
            break;
    }
    window.display();
}

void Shell::loadNewLevel()
{
    state_main = Main_State::LOADING;
    loading_screen.prepare(&game, &ui, window);
}

void Shell::alignState()
{
    if (change_main) {
        change_main = false;
        switch (state_main) {
            case Main_State::QUIT:
                window.close();
                break;
            case Main_State::NEWGAME:
                loadNewLevel();
                break;
            case Main_State::GAME:
                break;
            default:
                break;
        }
    }
    if (change_menu) {
        change_menu = false;
        switch (state_menu) {
            case Menu_State::MAIN:
                menu = &menu_main;
                break;
            case Menu_State::PAUSE:
                menu = &menu_pause;
                break;
            case Menu_State::SETTINGS:
                menu = &menu_settings;
                break;
            case Menu_State::KEYS:
                menu = &menu_input;
                break;
            default:
                break;
        }
        input.menuChange();
    }
}
