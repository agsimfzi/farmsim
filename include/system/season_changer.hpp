#pragma once

#include <SFML/Graphics.hpp>

#include <future>
#include <thread>
#include <vector>

#include <game/game.hpp>

#include <ui/button.hpp>
#include <ui/ui.hpp>

#include "state_hook.hpp"

class Season_Changer : public State_Hook, public sf::Drawable {
public:
    Season_Changer(Game& game, UI& ui);
    void update();
    void clickLeft();

private:
    Button b_continue;
    std::vector<Button> buttons;

    enum State {
        WAITING,
        RUNNING,
        COMPLETE
    };

    State state = WAITING;

    std::thread thread;

    std::future<void> omen;

    std::packaged_task<void()> task;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
