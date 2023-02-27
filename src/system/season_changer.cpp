#include <system/season_changer.hpp>

#include <chrono>

#include <resources/font_manager.hpp>

#include <util/fmouse.hpp>

Season_Changer::Season_Changer(Game& game, UI& ui)
    : task { std::bind(&Game::nextSeason, &game) }
{
    change = [&]() { game.nextSeason(); };
    auto func = [&]() { newMain(Main_State::GAME); state = WAITING; };
    b_continue = Button("continue", Font_Manager::get(Font::MENU), func, 56);
    b_continue.setPosition(sf::Vector2f(1660.f, 960.f));

    uiRead = std::bind(&UI::readSeasonChange, &ui);
}

void Season_Changer::update()
{
    switch (state) {
        case WAITING:
            task = std::packaged_task<void()>(change);
            b_continue.unsetAvailable();
            omen = task.get_future();
            thread = std::thread(std::move(task));
            state = RUNNING;
            break;
        case RUNNING:
            if (thread.joinable()) {
                auto status = omen.wait_for(std::chrono::milliseconds(0));
                if (status == std::future_status::ready) {
                    thread.join();
                    b_continue.setAvailable();
                    state = COMPLETE;
                }
            }
            break;
        case COMPLETE:
            uiRead();
            break;
        default:
            // ...?
            break;
    }

// buttons are checked after state parsing for same-frame highlighting of newly-available buttons
    sf::Vector2f mpos = fMouse();
    b_continue.update(mpos);
    for (auto& b : buttons) {
        b.update(mpos);
    }
}

void Season_Changer::clickLeft()
{
    if (!b_continue.click()) {
        for (auto& b : buttons) {
            if (b.click()) {
                return;
            }
        }
    }
}

void Season_Changer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(b_continue, states);
    for (const auto& b : buttons) {
        target.draw(b, states);
    }
}
