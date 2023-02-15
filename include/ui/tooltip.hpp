#pragma once

#include <SFML/Graphics.hpp>

class Tooltip : public sf::Drawable {
public:
    Tooltip() = default;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
