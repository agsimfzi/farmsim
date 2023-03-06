#pragma once

#include <SFML/Graphics.hpp>

#include "rock_data.hpp"

class Rock : private Rock_Data {
public:
    enum Type {
        GRANITE,
        LIMESTONE,
        BASALT,
        COPPER,
        IRON,
        GOLD,
        NULL_TYPE
    };

    Rock() = default;
    Rock(sf::Vector2i coordinates, const sf::Texture& texture, const Rock::Type type, Rock_Data d);

    bool dead();
    void hit(int dmg);
    void resetHealth();

    const Type type;

    std::string getName();
    std::string product();
    unsigned int quantity();

    static Rock::Type stringToType(std::string s);

    sf::Sprite sprite;

private:
    sf::Vector2i coordinates;

    short int health = 50;
    short int max_health = 50;
};
