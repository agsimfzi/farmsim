#pragma once

#include <string>

enum class Floor_Type {
    DIRT = 0,
    TILLED,
    WATERED,
    SAND,
    BASALT,
    GRASS,
    WATER,
    LAVA,
    NULL_TYPE
};

enum class Detail_Type {
    NULL_TYPE
};

inline std::string floorTypeToString(Floor_Type t)
{
    switch(t) {
        case Floor_Type::DIRT:
            return "DIRT";
        case Floor_Type::TILLED:
            return "TILLED";
        case Floor_Type::WATERED:
            return "WATERED";
        case Floor_Type::SAND:
            return "SAND";
        case Floor_Type::BASALT:
            return "BASALT";
        case Floor_Type::GRASS:
            return "GRASS";
        case Floor_Type::WATER:
            return "WATER";
        case Floor_Type::LAVA:
            return "LAVA";
        default:
            return "";
    }
}

inline std::string detailTypeToString(Detail_Type t)
{
    switch(t) {
        default:
            return "";
    }
}
