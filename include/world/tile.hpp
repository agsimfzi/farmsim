#pragma once

#include <map>
#include <memory>

#include "biome.hpp"
#include "building.hpp"
#include "detail.hpp"
#include "tile_type.hpp"
#include "tile_size.hpp"

template <class T>
using Map_Tile = std::map<int, std::map<int, T>>;

/// TILE INFO ///
///
/// \brief POD defining a world tile
/// rename this when you get a chance
///
struct Tile_Info {
    Tile_Info() = default;

    Tile_Info(const Tile_Info& i);

    sf::Vector2i coordinates; /**< world coordinates */

    bool planted = false; /**< tracks crops */
    Biome biome{ Biome::NULL_TYPE }; /**< base tile biome. null type makes an empty tile. */

    std::unique_ptr<Detail> detail;
    std::shared_ptr<Building> building { nullptr }; /**< stores a pointer to a local building */

    Floor_Type floor{ Floor_Type::NULL_TYPE }; /**< base tile type */
    sf::Vector2i texture_pos{ 0, 0 }; /**< textureRect pos */

    void setFloor(Floor_Type t);
};
