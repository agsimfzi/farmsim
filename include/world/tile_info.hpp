#pragma once

#include <memory>

#include "biome.hpp"
#include "building.hpp"
#include "rock.hpp"
#include "tile_type.hpp"
#include "tree.hpp"

/// FLOOR INFO ///
///
/// \brief POD defining a world tile
///
struct Floor_Info {
    sf::Vector2i coordinates; /**< world coordinates */
    bool planted = false; /**< tracks crops */
    Biome biome{ Biome::NULL_TYPE }; /**< base tile biome. null type makes an empty tile. */
    Floor_Type floor{ Floor_Type::NULL_TYPE }; /**< base tile type */
    Detail_Type detail{ Detail_Type::NULL_TYPE }; /**< currently unused, but will eventually serve to cover rocks, trees, and shrubs. */
    sf::Vector2i texture_pos; /**< textureRect pos */
    sf::Vector2i detail_pos; /**< textureRect size */
    Tree::Type tree { Tree::Type::NULL_TYPE }; /**< tracks trees, to be deprecated */
    std::shared_ptr<Rock> rock { nullptr }; /**< tracks rocks, to be deprecated */
    std::shared_ptr<Building> building { nullptr }; /**< stores a pointer to a local building */
};
