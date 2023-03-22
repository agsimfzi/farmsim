#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <animation/animated_sprite.hpp>

#include "tile_type.hpp"
#include "vehicle_data.hpp"

/// VEHICLE ///
///
/// \brief world object that's ridable by the player
///
struct Vehicle : public Animated_Sprite<Vehicle_State> {

/// TYPE ///
///
/// \brief enumerated vehicle type. available:
/// BOAT
/// BROOM
///
    enum Type{
        BOAT,
        BROOM,
        NULL_TYPE
    };

    Vehicle() = default;

/// FULL CONSTRUCTOR ///
///
/// \brief creates the vehicle using a position and a Vehicle_Data struct
    Vehicle(sf::Vector2f pos, Vehicle_Data d);

/// stringToType ///
///
/// \brief converts passed string to enumerated vehicle type
    static Vehicle::Type stringToType(std::string s);

    Type type;
    float speed_factor; /**< multiplicative speed factor applied to any creature riding this vehicle */
};
