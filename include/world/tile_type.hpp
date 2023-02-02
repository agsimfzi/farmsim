#pragma once

enum class Floor_Type {
    DIRT = 0,
    TILLED,
    WATERED,
    SAND,
    NULL_TYPE
};

enum class Detail_Type {
    GRASS,
    WATER,
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
        default:
            return "";
    }
}

inline std::string detailTypeToString(Detail_Type t)
{
    switch(t) {
        case Detail_Type::GRASS:
            return "GRASS";
        case Detail_Type::WATER:
            return "WATER";
        default:
            return "";
    }
}
