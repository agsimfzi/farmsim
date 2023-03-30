#pragma once

#include <SFML/Graphics.hpp>

/// ENERGY BAR ///
///
/// \brief UI class which represents the player's energy as a meter
///
class Energy_Bar : public sf::Drawable {
public:
/// DEFAULT CONSTRUCTOR ///
///
/// \brief initializes the frame
///
    Energy_Bar();

/// setPosition ///
///
/// \brief places the frame and fill objects
///
    void setPosition(sf::Vector2f pos);

/// update ///
///
/// \brief updates the fill and checks its color, if energy has changed
///
    void update(float energy);

    sf::Vector2f getSize();

private:
    sf::RectangleShape frame; /**< the shell the meter fills */
    sf::RectangleShape fill; /**< represents the meter itself */

    const float height = 48.f; /**< frame and fill y size */
    const float max_size = 192.f; /**< frame x size, max for fill */

    const float outline_thickness = 2.f; /**< for the frame */

    float last_energy = -1.f; /**< for checking if energy has changed (and thus avoiding superfluous updating) */

/// calculateFillColor ///
///
/// \brief calculates the color of the fill. currently unimplemented, but should go from green to yellow to red.
///
    sf::Color calculateFillColor(float fill);

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
