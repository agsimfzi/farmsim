#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>

#include "season.hpp"

class Crop_Data {
public:
    size_t uid;
    std::string name;
    std::string description;

    sf::Vector2i coordinates;

    Season season;

    float growth_coef;
};
