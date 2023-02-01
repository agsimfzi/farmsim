#pragma once

enum class Floor_Type {
    DIRT = 0,
    TILLED,
    WATERED,
    WATER,
    NULL_TYPE
};

enum class Detail_Type {
    GRASS,
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
        case Floor_Type::WATER:
            return "WATER";
        default:
            return "";
    }
}
