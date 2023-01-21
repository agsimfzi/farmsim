#include <audio/sound_bus.hpp>

//////////////////////////////////////////////////////////////

std::queue<Sound_Game> Sound_Bus::next_game_sound;
std::queue<Sound_UI> Sound_Bus::next_ui_sound;

void Sound_Bus::addSound(Sound_Game sound)
{
    next_game_sound.push(sound);
}

void Sound_Bus::addSound(Sound_UI sound)
{
    next_ui_sound.push(sound);
}

Sound_Game Sound_Bus::nextGameSound()
{
    if (!next_game_sound.empty()) {
        Sound_Game s = next_game_sound.front();
        next_game_sound.pop();
        return s;
    }
    else {
        return Sound_Game::NULL_SOUND;
    }
}

Sound_UI Sound_Bus::nextUISound()
{
    if (!next_ui_sound.empty()) {
        Sound_UI s = next_ui_sound.front();
        next_ui_sound.pop();
        return s;
    }
    else {
        return Sound_UI::NULL_SOUND;
    }
}
