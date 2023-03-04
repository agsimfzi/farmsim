#pragma once

#include <SFML/Graphics.hpp>

#include "season.hpp"

class Tree : public sf::Sprite {
public:
    enum Type {
        PINE,
        BIRCH,
        NULL_TYPE
    };

    Tree() = default;
    Tree(sf::Vector2i coordinates, const sf::Texture& texture, Tree::Type type);

    bool dead();
    void hit(int dmg);
    void resetHealth();

    Type type;

    short int health = 50;

    void setSeason(Season s);

private:
    sf::Vector2i coordinates;
    short int max_health = 50;
};
