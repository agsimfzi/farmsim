#pragma once

#include <SFML/Graphics.hpp>

class Wallet_Inspector : public sf::Drawable {
public:
    Wallet_Inspector();

    void update(int count);

private:
    sf::RectangleShape frame;
    sf::Sprite icon;
    sf::Text text;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
