#pragma once

#include <string>

/// TILE TYPE ///
///
/// \brief enumerated floor type, for selecting sprites and performing world interactions.
///
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

/// DETAIL TYPE ///
///
/// \brief deprecate the shit out of this
///
enum class Detail_Type {
    NULL_TYPE
};

/// floorTypeToString ///
///
/// \brief convert passed enumerated floor type to a string
///
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

/// detailTypeToString ///
///
/// \brief deprecate the heck out of this
///
inline std::string detailTypeToString(Detail_Type t)
{
    switch(t) {
        default:
            return "";
    }
}
