#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>

#include <item/item_data.hpp>

#include "season.hpp"

class Crop_Data {
public:
    Crop_Data() = default;
    size_t uid;
    std::string name;

    Season season;

    float growth_coef;
    float water_factor;
    size_t stage_count;
    int y_size;
};
