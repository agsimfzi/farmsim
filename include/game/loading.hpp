#pragma once

#include <SFML/Graphics.hpp>

#include <future>
#include <map>
#include <thread>

#include <world/world.hpp>

/////////////////////////////////////////////////////////////
/// \brief
///
//loading screen should:
//display stats from previous level, and stats for the entire run (except the first loading screen!)
//have a "continue" button/hotkey which can be activated when loading is done

class Loading_Screen : public sf::Drawable {
public:
    Loading_Screen();

    bool update();

    bool isFinished();

    void prepare(std::vector<std::function<void()>> toLoad, std::vector<std::string> nmessages);

private:
    std::thread thread;

    unsigned int step;

    sf::RectangleShape backdrop;
    sf::Text text;

    void finish();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    enum State {
        LOADING,
        WAITING,
        END
    };

    State state;

    bool finished;

    void doStep();

    std::vector<std::packaged_task<void()>> tasks;
    std::vector<std::string> messages;

    std::future<void> omen;
};
