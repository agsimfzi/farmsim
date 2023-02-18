#pragma once

#include "world/tile_type.hpp"

#include <SFML/Graphics/Sprite.hpp>

struct Vehicle : public sf::Sprite {
    enum Type{
        BOAT,
        BROOM,
        NULL_VEHICLE
    };

    Vehicle() = default;
    Vehicle(Vehicle::Type type, sf::Vector2f pos);

    Type type;
};
