#pragma once

#include <SFML/Graphics.hpp>

class Energy_Bar : public sf::Drawable {
public:
    Energy_Bar();

    void setPosition(sf::Vector2f pos);
    void update(float energy);

private:
    sf::RectangleShape frame;
    sf::RectangleShape fill;

    const float height = 48.f;
    const float max_size = 192.f;

    const float outline_thickness = 2.f;

    sf::Color calculateFillColor(float fill);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
