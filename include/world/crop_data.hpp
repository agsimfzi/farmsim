#pragma once

#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>

#include "season.hpp"

/// CROP DATA ///
///
/// \brief stores data for a single crop
///
class Crop_Data {
public:

/// DEFAULT CONSTRUCTOR ///
    Crop_Data() = default;

    size_t uid; /**< unique identifier. identical to the crop's seed */
    std::string name;

    float growth_coef; /**< the speed at which the crop grows */
    float water_factor; /**< the multiplicative factor by which watering increases the growth coefficient */
    size_t stage_count; /**< number of growth stages */
    int y_size; /**< sprite height */

    std::map<Season, bool> seasons; /**< stores season validation */

    bool m_regrows; /**< tracks crops which revert to the penultimate stage on harvest, rather than disappear */

    unsigned int quantity_min; /**< lowest number of harvested crops */
    unsigned int quantity_max; /**< highest number of harvested crops */

    bool m_passable = true; /**< enables collision if false */
};
