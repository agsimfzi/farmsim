#pragma once

#include <SFML/Graphics.hpp>

class Building : public sf::Sprite {
public:
    Building() = default;

    bool active = false;

    enum Type {
        FURNACE,
        NULL_TYPE
    };
};
