#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include <SFML/System/Clock.hpp>

#include "sound.hpp"

/////////////////////////////////////////////////////////////
/// \brief Converts Sound_Game and Sound_UI types into sf::SoundBuffer objects,
/// which are then passed to Sound_Player to be constructed as sf::Sound
///
struct Sound_Context {
    Sound_Context() = default;
    Sound_Context(sf::SoundBuffer buffer, float threshold)
        : buffer { buffer }
        , threshold { threshold }
    {
    }

    sf::SoundBuffer buffer;
    sf::Clock clock;
    float threshold;

    bool ready()
    {
        if (clock.getElapsedTime().asSeconds() >= threshold) {
            clock.restart();
            return true;
        }
        return false;
    }
};
