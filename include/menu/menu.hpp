#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include <audio/audio_settings.hpp>

#include <system/settings_package.hpp>
#include <system/state_hook.hpp>

#include "menu_elements.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Menu : public sf::Drawable, public State_Hook {
public:
    Menu();
    virtual void update(sf::Vector2f mpos);
    virtual void clickLeft();
    virtual void clickRight();
    void releaseLeft();
    void releaseRight();
    virtual void back();
    void scroll(float delta);
    virtual void saveSettings() { }
    void stopInput();

protected:
    std::vector<Option> options;
    std::map<Volume_Type, Slider> sliders;

    Main_State escape = Main_State::NULL_STATE;
    static sf::Font& font;

    std::vector<Logo> logos;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    static std::vector<Nav> nav;
};

/////////////////////////////////////////////////////////////
/// \brief
///
class Menu_Main : public Menu {
public:
    Menu_Main();

protected:
private:
};

/////////////////////////////////////////////////////////////
/// \brief
///
class Menu_Pause : public Menu {
public:
    Menu_Pause();

protected:
private:
};

/////////////////////////////////////////////////////////////
/// \brief
///
class Menu_Settings : public Menu, public Audio_Settings {
public:
    Menu_Settings();
    virtual void back() override;
    void saveSettings();

protected:
    Slider volume_music { "music" };
    Slider volume_sound { "sound" };

private:
    Audio_Settings audioSettings;

    void reset();

    Settings_Package generateSettingsPackage();
    void loadSettings();
};

/////////////////////////////////////////////////////////////
/// \brief
///
class Menu_Input : public Menu, public Input_Mapper {
public:
    Menu_Input();
    virtual void back() override;

    virtual void update(sf::Vector2f mpos) override;

    virtual void clickLeft() override;
    virtual void clickRight() override;

    void setActions(const std::map<std::string, Action>& actions);

    void save();
    void cancel();

    std::function<void(const std::vector<Action>)> saveActions;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

/////////////////////////////////////////////////////////////
/// \brief
///
struct Menu_Package {
    Menu_Package(Menu_Main* m, Menu_Pause* p, Menu_Settings* s, Menu_Input* k)
        : m_main { m }
        , m_pause { p }
        , m_settings { s }
        , m_keymap { k }
    {
    }

    Menu_Main* m_main;
    Menu_Pause* m_pause;
    Menu_Settings* m_settings;
    Menu_Input* m_keymap;
};
