#include <audio/sound_player.hpp>

//////////////////////////////////////////////////////////////

float Sound_Player::volume_game = 100.f;

float Sound_Player::volume_ui = 100.f;

std::deque<sf::Sound> Sound_Player::sounds_game = std::deque<sf::Sound>();
std::deque<sf::Sound> Sound_Player::sounds_ui = std::deque<sf::Sound>();

void Sound_Player::addGameSound(const sf::SoundBuffer& sound)
{
    sounds_game.push_back(sf::Sound(sound));
    sounds_game.back().setVolume(volume_game);
    sounds_game.back().play();
}

void Sound_Player::addUISound(const sf::SoundBuffer& sound)
{
    sounds_ui.push_back(sf::Sound(sound));
    sounds_ui.back().setVolume(volume_ui);
    sounds_ui.back().play();
}

void Sound_Player::update()
{
    while (!sounds_game.empty() && sounds_game.front().getStatus() == sf::Sound::Stopped) {
        sounds_game.pop_front();
    }
    while (!sounds_game.empty() && sounds_game.front().getStatus() == sf::Sound::Stopped) {
        sounds_game.pop_front();
    }
}
