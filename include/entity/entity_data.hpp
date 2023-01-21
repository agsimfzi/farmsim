#pragma once

#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>

#include "entity_state.hpp"

struct Entity_Data {
    std::string name;
    std::string description;
    float speed;
    sf::Vector2i size;
    std::map<Entity_State, unsigned int> aCount;
    std::map<Entity_State, int> aThreshold;
};
