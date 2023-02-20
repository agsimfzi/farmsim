#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>
#include <map>

enum class Vehicle_State {
    IDLE,
    MOVING
};

struct Vehicle_Data {
    std::string name;
    float speed_factor{ 1.f };
    sf::Vector2i size;
    std::map<Vehicle_State, unsigned int> counts;
    std::map<Vehicle_State, int> thresholds;
};
