#pragma once

#include <SFML/Audio/Sound.hpp>

#include <deque>

//sounds by kenney.nl

/////////////////////////////////////////////////////////////
/// \brief
///
class Sound_Player {
public:
    Sound_Player() = default;

    void addGameSound(const sf::SoundBuffer& sound);
    void addUISound(const sf::SoundBuffer& sound);

    void update();

protected:
    static float volume_game;
    static float volume_ui;
    static std::deque<sf::Sound> sounds_game;
    static std::deque<sf::Sound> sounds_ui;
};
