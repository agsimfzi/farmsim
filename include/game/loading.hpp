#pragma once

#include <SFML/Graphics.hpp>

#include <future>
#include <map>
#include <thread>

#include <game/game.hpp>

#include <ui/ui.hpp>

/// LOADING SCREEN ///
///
/// \brief spawns threads to perform game loading while allowing for discrete rendering
///

class Loading_Screen : public sf::Drawable {
public:

/// DEFAULT CONSTRUCTOR ///
///
/// \brief defines backdrop and text
///
    Loading_Screen();

/// update ///
///
/// \brief checks the thread and
///
    bool update();

    bool isFinished();

    void prepare(Game* game, UI* ui, sf::RenderWindow& window);
    void prepare(std::vector<std::function<void()>> toLoad, std::vector<std::string> nmessages);

private:
    std::thread thread; /**<  */

    unsigned int step; /**<  */

    sf::RectangleShape backdrop; /**<  */
    sf::Text text; /**<  */

/// STATE ///
///
/// \brief
///
    enum State {
        LOADING,
        WAITING,
        END
    };

    State state; /**<  */

    bool finished; /**<  */

    void doStep();

    std::vector<std::packaged_task<void()>> tasks; /**<  */
    std::vector<std::string> messages; /**<  */

    std::future<void> omen; /**<  */

    void finish();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
