#pragma once

#include "biome.hpp"
#INCLUDE "building.hpp"
#include "tile_type.hpp"
#include "tree.hpp"

struct Floor_Info {
    sf::Vector2i coordinates;
    bool planted = false;
    Biome biome;
    Floor_Type floor;
    Detail_Type detail{ Detail_Type::NULL_TYPE };
    sf::Vector2i texture_pos;
    sf::Vector2i detail_pos;
    bool tree = false;
    bool rock = false;
    Building::Type building = Building::NULL_TYPE;
};
