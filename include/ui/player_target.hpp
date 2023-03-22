#pragma once

#include <SFML/Graphics.hpp>

/// PLAYER TARGET ///
///
/// \brief UI element drawing a frame around the currently targeted tile
///
class Player_Target : public sf::Drawable {
public:
/// DEFAULT CONSTRUCTOR ///
///
/// \brief defines the frame
///
    Player_Target();

    bool active = false; /**< tracks whether the target may be drawn */

/// set ///
///
/// \brief set active true and place the frame
///
    void set(const sf::Vector2i& coords);

private:
    sf::RectangleShape frame; /**< visual target element */

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
