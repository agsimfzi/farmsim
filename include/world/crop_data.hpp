#pragma once

#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>

#include "season.hpp"

class Crop_Data {
public:
    Crop_Data() = default;
    size_t uid;
    std::string name;

    float growth_coef;
    float water_factor;
    size_t stage_count;
    int y_size;

    std::map<Season, bool> seasons;

    bool m_regrows;

    unsigned int quantity_min;
    unsigned int quantity_max;

    bool m_passable = true;
};
