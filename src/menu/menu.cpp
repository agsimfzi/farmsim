#include <menu/menu.hpp>

#include <audio/sound_bus.hpp>
#include <audio/sound.hpp>

#include <resources/font_manager.hpp>

#include <system/database.hpp>

//////////////////////////////////////////////////////////////

std::vector<Nav> Menu::nav = std::vector<Nav>();

sf::Font& Menu::font = Font_Manager::get(Font::MENU);

Menu::Menu()
{
    sf::Vector2f pos(256.f, 128.f);

    nav.push_back(Nav("new game", font, Main_State::NEWGAME, Menu_State::PAUSE));
    nav.back().setPosition(pos);
    pos.y += 128.f;

    nav.push_back(Nav("settings", font, Main_State::MENU, Menu_State::SETTINGS));
    nav.back().setPosition(pos);
    pos.y += 128.f;

    nav.push_back(Nav("quit", font, Main_State::QUIT, Menu_State::NULL_STATE));
    nav.back().setPosition(pos);

    logos.push_back(Logo("https://github.com/surfactants/", sf::Vector2f(1100.f, 900.f), "SURFACTANT"));

    logos.push_back(Logo("https://www.sfml-dev.org", sf::Vector2f(1600.f, 900.f), "SFML"));
}

void Menu::clickLeft()
{
    for (auto& button : nav) {
        if (button.isHighlighted()) {
            Sound_Bus::addSound(Sound_UI::TRIGGER_BUTTON);
            if (button.target_main != Main_State::NULL_STATE) {
                newMain(button.target_main);
            }
            if (button.target_menu != Menu_State::NULL_STATE) {
                newMenu(button.target_menu);
            }
            return;
        }
    }
    for (auto& option : options) {
        if (option.isHighlighted()) {
            Sound_Bus::addSound(Sound_UI::TRIGGER_BUTTON);
            option.target();
            return;
        }
    }
    for (auto& slider : sliders) {
        if (slider.second.click()) {
            Sound_Bus::addSound(Sound_UI::TRIGGER_BUTTON);
            return;
        }
    }
    for (auto& logo : logos) {
        if (logo.click()) {
            Sound_Bus::addSound(Sound_UI::TRIGGER_BUTTON);
            return;
        }
    }
}

void Menu::clickRight()
{
}

void Menu::releaseLeft()
{
    for (auto& slider : sliders) {
        if (slider.second.unclick()) {
            Sound_Bus::addSound(Sound_UI::TRIGGER_BUTTON);
            return;
        }
    }
}

void Menu::scroll(float delta)
{
    for (auto& slider : sliders) {
        if (slider.second.checkMouse()) {
            slider.second.scroll(delta);
            break;
        }
    }
}

void Menu::releaseRight() { }

void Menu::update(sf::Vector2f mpos)
{
    for (auto& button : nav) {
        button.update(mpos);
    }
    for (auto& option : options) {
        option.update(mpos);
    }
    for (auto& slider : sliders) {
        slider.second.update();
    }
    for (auto& logo : logos) {
        logo.update(mpos);
    }
}

void Menu::back()
{
    if (escape != Main_State::NULL_STATE) {
        newMain(escape);
    }
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& button : nav) {
        target.draw(button, states);
    }
    for (const auto& option : options) {
        target.draw(option, states);
    }
    for (const auto& slider : sliders) {
        target.draw(slider.second, states);
    }
    for (const auto& logo : logos) {
        target.draw(logo);
    }
}

Menu_Main::Menu_Main()
{
    escape = Main_State::QUIT;
}

Menu_Pause::Menu_Pause()
{
    escape = Main_State::GAME;
}

Menu_Settings::Menu_Settings()
{
    sliders[Volume_Type::MUSIC] = Slider("music volume");

    sliders[Volume_Type::GAME] = Slider("game volume");

    sliders[Volume_Type::UI] = Slider("ui volume");

    sf::Vector2f spos(544.f, 0.f);

    for (auto& slider : sliders) {
        spos.y += 128.f;
        slider.second.set(spos, font);
    }

    spos.x += 32.f;
    spos.y += 256.f;

    options.push_back(Option("input", font, [&]() { state_menu = Menu_State::KEYS; change_menu = true; }));
    options.back().setPosition(spos);

    spos.y += 128.f;

    options.push_back(Option("save", font, std::bind(&Menu::saveSettings, this)));
    options.back().setPosition(spos);

    spos.x += 192.f;

    options.push_back(Option("cancel", font, std::bind(&Menu::back, this)));
    options.back().setPosition(spos);

    loadSettings();
}

void Menu_Settings::back()
{
    reset();
    newMenu(prev_menu);
}

void Menu_Settings::saveSettings()
{
    setVolumeMusic(sliders[Volume_Type::MUSIC].getFill());
    setVolumeGame(sliders[Volume_Type::GAME].getFill());
    setVolumeUI(sliders[Volume_Type::UI].getFill());
    Database::saveSettings(generateSettingsPackage());
    back();
}

Settings_Package Menu_Settings::generateSettingsPackage()
{
    Settings_Package p;

    p.volume[Volume_Type::MUSIC] = getVolumeMusic();
    p.volume[Volume_Type::GAME] = getVolumeGame();
    p.volume[Volume_Type::UI] = getVolumeUI();

    return p;
}

void Menu_Settings::reset()
{
    sliders[Volume_Type::MUSIC].setFill(getVolumeMusic());
    sliders[Volume_Type::GAME].setFill(getVolumeGame());
    sliders[Volume_Type::UI].setFill(getVolumeUI());
}

void Menu_Settings::loadSettings()
{
    Settings_Package p = Database::getSettings();
    setVolumeMusic(p.volume[Volume_Type::MUSIC]);
    setVolumeGame(p.volume[Volume_Type::GAME]);
    setVolumeUI(p.volume[Volume_Type::UI]);
    reset();
}

void Menu::stopInput()
{
    releaseLeft();
}

Menu_Input::Menu_Input()
{
    sf::Vector2f spos(512.f, 192.f);

    setPosition(spos);

    spos.x += 512.f + 128.f;

    options.push_back(Option("save", font, std::bind(&Menu_Input::save, this)));
    options.back().setPosition(spos);

    spos.y += 128.f;

    options.push_back(Option("reset", font, std::bind(&Input_Mapper::reset, this)));
    options.back().setPosition(spos);

    spos.y += 128.f;

    options.push_back(Option("cancel", font, std::bind(&Menu_Input::back, this)));
    options.back().setPosition(spos);
}

void Menu_Input::back()
{
    reset();
    state_menu = Menu_State::SETTINGS;
    change_menu = true;
}

void Menu_Input::save()
{
    confirm();
    saveActions(getActions());
    Database::saveActions(getActions());
    back();
}

void Menu_Input::update(sf::Vector2f mpos)
{
    Menu::update(mpos);
    checkMouse(mpos);
}

void Menu_Input::clickLeft()
{
    if (!Input_Mapper::clickLeft()) {
        Menu::clickLeft();
    }
}

void Menu_Input::clickRight()
{
    if (!Input_Mapper::clickRight()) {
        Menu::clickRight();
    }
}

void Menu_Input::setActions(const std::map<std::string, Action>& actions)
{
    Input_Mapper::setActions(font, actions);
}

void Menu_Input::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Menu::draw(target, states);
    Input_Mapper::draw(target, states);
}
