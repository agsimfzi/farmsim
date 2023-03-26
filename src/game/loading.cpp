#include <game/loading.hpp>

#include <chrono>
#include <functional>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>

//////////////////////////////////////////////////////////////

Loading_Screen::Loading_Screen()
{
    text.setFont(Font_Manager::get(Font::MENU));
    text.setFillColor(Palette::white);
    text.setPosition(sf::Vector2f(128.f, 128.f));
    text.setCharacterSize(48);

    backdrop.setFillColor(Palette::black);
    backdrop.setSize(sf::Vector2f(1920.f, 1080.f));
}

bool Loading_Screen::update()
{
    if (state == LOADING) {
        if (thread.joinable()) {
            auto status = omen.wait_for(std::chrono::milliseconds(0));
            if (status == std::future_status::ready) {
                thread.join();
                doStep();
            }
        }
    }
    else if (state == WAITING) {
        //check for clicks on the continue button and perform the final animation
        state = END;
    }

    return (state == END);
}

void Loading_Screen::doStep()
{
    if (step < tasks.size()) {
        omen = tasks[step].get_future();
        thread = std::thread(std::move(tasks[step]));
        text.setString(messages[step]);
    }
    else {
        state = WAITING;
    }

    step++;
}

void Loading_Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backdrop, states);
    target.draw(text, states);
}

void Loading_Screen::prepare(Game* game, UI* ui, sf::RenderWindow& window)
{
    tasks.clear();
    messages.clear();

    //SKIPPING THE FIRST STEP ALLOWS THE RENDERING TO SWAP OVER TO LOADING BEFORE IT BEGINS
    tasks.push_back(std::packaged_task<void()>([](){}));//std::function<void()>([] {}));
    messages.push_back("...");

    World* world = &game->getWorld();

    tasks.push_back(std::packaged_task<void()>(std::bind(&World::reset, world)));
    messages.push_back("resetting world...");

    tasks.push_back(std::packaged_task<void()>(std::bind(&World::makeBiomes, world)));
    messages.push_back("making biomes...");

    tasks.push_back(std::packaged_task<void()>(std::bind(&World::makeGrass, world)));
    messages.push_back("making grass...");

    tasks.push_back(std::packaged_task<void()>(std::bind(&UI::init, ui)));
    messages.push_back("initializing UI.");

    tasks.push_back(std::packaged_task<void()>(std::bind(&Game::startGame, game)));
    messages.push_back("finalizing world!");

    auto grab = [&]() { window.requestFocus(); };
    tasks.push_back(std::packaged_task<void()>(grab));
    messages.push_back("!");

    step = 0;
    doStep();
}
