#pragma once

#include <SFML/Graphics.hpp>

class Rock : public sf::Sprite {
public:
    Rock() = default;
    Rock(sf::Vector2i coordinates, const sf::Texture& texture);

    bool dead();
    void hit(int dmg);
    void resetHealth();

private:
    sf::Vector2i coordinates;

    short int health = 100;
    short int max_health = 100;
};
