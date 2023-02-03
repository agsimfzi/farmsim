#pragma once

#include <SFML/Graphics.hpp>

#include "tile.hpp"

class Tree : public sf::Sprite {
public:
    Tree() = default;
    Tree(sf::Vector2i coordinates, const sf::Texture& texture);

    bool dead();
    void hit(int dmg);
    void resetHealth();

    enum Type {
        BIRCH,
        PINE
    };

private:
    sf::Vector2i coordinates;

    short int health = 100;
    short int max_health = 100;
};
