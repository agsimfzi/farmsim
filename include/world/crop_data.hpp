#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>

#include <item/item_data.hpp>

#include "season.hpp"

class Crop_Data {
public:
    Crop_Data() = default;
    Crop_Data(Item_Data d);
    size_t uid;
    std::string name;
    std::string description;

    sf::Vector2i coordinates;

    Season season;

    float growth_coef;
};
