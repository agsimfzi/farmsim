#pragma once

#include <SFML/Graphics.hpp>

class Player_Target : public sf::Drawable {
public:
    Player_Target();

    bool active = false; //debug, set to false later

    void set(const sf::Vector2i& coords);

private:
    sf::RectangleShape frame;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
