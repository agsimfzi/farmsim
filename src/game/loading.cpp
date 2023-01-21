#include <game/loading.hpp>

#include <chrono>
#include <functional>

#include <resources/font_manager.hpp>

//////////////////////////////////////////////////////////////

Loading_Screen::Loading_Screen()
{
    text.setFont(Font_Manager::get(Font::MENU));
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(128.f, 128.f));
    text.setCharacterSize(48);

    backdrop.setFillColor(sf::Color::Black);
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
        //check for clicks on the continue button
        state = END;
    }
    else if (state == END) { //TODO: check for the end of the final animation
        finish();
    }

    return finished;
}

void Loading_Screen::doStep()
{
    if (step < tasks.size()) {
        omen = tasks[step].get_future();
        thread = std::thread(std::move(tasks[step]));
        text.setString(messages[step]);
    }
    else {
        state = END;
    }

    step++;
}

void Loading_Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backdrop, states);
    target.draw(text, states);
}

void Loading_Screen::prepare(std::vector<std::function<void()>> toLoad, std::vector<std::string> nmessages)
{
    messages = nmessages;
    state = LOADING;

    for (const auto& it : toLoad) {
        tasks.push_back(std::packaged_task<void()>(it));
    }

    step = 0;
    finished = false;
    doStep();
}

void Loading_Screen::finish()
{
    tasks.clear();
    messages.clear();
    finished = true;
}
