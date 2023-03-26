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
/// \brief checks for and responds to thread termination
///
/// \return true if state == END
    bool update();


/// prepare ///
///
/// \brief loads functions to be performed when loading a game ///
///
    void prepare(Game* game, UI* ui, sf::RenderWindow& window);

private:
    std::thread thread; /**< for running loading operations concurrently */

    unsigned int step; /**< tracks the vector index of the current task */

    sf::RectangleShape backdrop; /**< background display */
    sf::Text text; /**< displays information about the current loading step */

/// STATE ///
///
/// \brief determines whether or not the game has been loaded, and if main state may be advanced
///
    enum State {
        LOADING,    // main state, for performing operations
        WAITING,    // pseudo end state, for final animation and reading "continue" option
        END         // indicates the state may be changed
    };

    State state; /**< current state */

/// doStep ///
///
/// \brief runs the next step in the loading process
///
    void doStep();

    std::vector<std::packaged_task<void()>> tasks; /**< loading functions. called in order. */
    std::vector<std::string> messages; /**< for text display during each operation */

    std::future<void> omen; /**< reads results of asynchronous loading tasks */

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
