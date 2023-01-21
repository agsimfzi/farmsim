#pragma once

#include "tile.hpp"

class Player_Target : public sf::Drawable {
public:
    Player_Target();

    bool active = true; //debug, set to false later

    void setActive(bool active);
    void place(sf::Vector2i coords);

    sf::Vector2i coordinates { 0, 0 };

private:
    sf::RectangleShape frame;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
