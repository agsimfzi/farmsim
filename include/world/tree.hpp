#pragma once

#include <SFML/Graphics.hpp>

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

    short int health = 50;

private:
    sf::Vector2i coordinates;
    short int max_health = 50;
};
