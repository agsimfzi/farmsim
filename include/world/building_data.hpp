#pragma once

/// BUILDING STATE ///
///
/// \brief tracks building states for animations
///
enum class Building_State {
    IDLE,
    STARTING,
    RUNNING,
    ENDING,
    NULL_STATE
};

/// Animation_Data ///
///
/// \brief obviously, this shouldn't be here. but it's a genericized version of what was previously here. will move it later.
///
template <typename K>
struct Animation_Data {
    size_t uid;
    std::map<K, unsigned int> counts; /**< number of frames per state animation */
    std::map<K, int> thresholds; /**< length of frames per state animation */
    sf::Vector2i start; /**< textureRect position of the first animation's first frame */
    std::string tkey; /**< texture key */
};

using Building_Animation_Data = Animation_Data<Building_State>; /**< explicit specialization which was mostly done to ease the genericizing refactor */
