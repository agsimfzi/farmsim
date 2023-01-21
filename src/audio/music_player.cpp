#include <audio/music_player.hpp>

#include <iostream>

float Music_Player::volume { 100.f };
float Music_Player::volume_tracker { 0.f };

sf::Music Music_Player::music;

Music_Player::Music_Player()
{
    music.setLoop(true);
}

void Music_Player::setSong(const std::string filename)
{
    if (filename != this->filename) {
            std::cout << "now playing " << filename << '\n';
        this->filename = filename;
        state = FADING_OUT;
        fadeClock.restart();
    }
}

void Music_Player::update()
{
    switch (state) {
        case FADING_OUT:
            if (fadeClock.getElapsedTime().asSeconds() >= fadeThreshold) {
                fadeOut();
                fadeClock.restart();
            }
            fadeOut();
            break;
        case FADING_IN:
            if (fadeClock.getElapsedTime().asSeconds() >= fadeThreshold) {
                fadeIn();
                fadeClock.restart();
            }
            break;
        case INTER:
            if (fadeClock.getElapsedTime().asSeconds() >= interThreshold) {
                music.openFromFile(filename);
                music.play();
                state = FADING_IN;
                fadeClock.restart();
            }
            break;
        default:
            break;
    }
}

void Music_Player::fadeOut()
{
    volume_tracker -= volumeOffset;
    if (volume_tracker < 0.f) {
        volume_tracker = 0.f;
        state = INTER;
    }
    music.setVolume(volume_tracker);
}

void Music_Player::fadeIn()
{
    volume_tracker += volumeOffset;
    if (volume_tracker > volume) {
        volume_tracker = volume;
        music.setVolume(volume);
        state = PLAYING;
    }
    music.setVolume(volume_tracker);
}
