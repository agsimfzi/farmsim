#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <animation/animated_sprite.hpp>

#include "tile_type.hpp"
#include "vehicle_data.hpp"

struct Vehicle : public Animated_Sprite<Vehicle_State> {
    enum Type{
        BOAT,
        BROOM,
        NULL_TYPE
    };

    Vehicle() = default;
    Vehicle(sf::Vector2f pos, Vehicle_Data d);

    static Vehicle::Type stringToType(std::string s);

    Type type;
    float speed_factor;
};
