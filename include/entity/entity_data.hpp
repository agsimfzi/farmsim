#pragma once

#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>

#include "entity_state.hpp"

/// ENTITY DATA ///
///
/// \brief POD defining a single entity
///
struct Entity_Data {
    std::string name;
    std::string description;
    float speed; /**< maximum velocity */
    sf::Vector2i size; /**< sprite frame size */
    std::map<Entity_State, unsigned int> animation_count; /**< number of frames per state animation */
    std::map<Entity_State, int> animation_threshold; /**< frame length per state animation */
    unsigned int bounds_y_offset; /**< custom bounds position from (centered) sprite position */
    sf::Vector2u bounds_size; /**< custom bounds size */
};
