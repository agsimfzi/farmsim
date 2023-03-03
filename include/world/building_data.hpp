#pragma once

enum class Building_State {
    IDLE,
    STARTING,
    RUNNING,
    ENDING,
    NULL_STATE
};

template <typename K>
struct Animation_Data {
    size_t uid;
    std::map<K, unsigned int> counts;
    std::map<K, int> thresholds;
    sf::Vector2i start;
    std::string tkey;
};

using Building_Animation_Data = Animation_Data<Building_State>;
