#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>

#include <deque>

#include "song.hpp"

//////////////////////////////////////////////////////////////

//music by me... eventually

class Music_Player {
public:
    Music_Player();

    void setSong(const std::string filename);

    void update();

    enum State { PAUSED,
        PLAYING,
        FADING_OUT,
        FADING_IN,
        INTER };

    State state;

protected:
    static float volume;
    static float volume_tracker;
    static sf::Music music;

private:
    sf::Clock fadeClock;
    float fadeThreshold { 0.075f };
    float interThreshold { 1.5f };

    float volumeOffset { 1.5f };

    std::string filename;

    void fadeOut();
    void fadeIn();
};
