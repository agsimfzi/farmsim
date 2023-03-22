#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>
#include <map>

/// VEHICLE STATE ///
///
/// \brief primarily for animations. possible values:
/// IDLE
/// MOVING
///
enum class Vehicle_State {
    IDLE,
    MOVING
};

/// VEHICLE DATA ///
///
/// \brief lightweight prototype used to construct Vehicles
///
struct Vehicle_Data {
    std::string name;
    float speed_factor{ 1.f }; /**< multiplicative factor by which the vehicle increases an entity's base speed */
    sf::Vector2i size; /**< sprite frame size */
    std::map<Vehicle_State, unsigned int> counts; /**< animation counts */
    std::map<Vehicle_State, int> thresholds; /**< animation thresholds */
};
