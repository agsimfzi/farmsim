#pragma once

#include <string>

/// VOLUME TYPE ///
///
/// \brief enumerated type for audio settings
///
enum class Volume_Type {
    MUSIC,
    GAME,
    UI,
    NULL_TYPE
};

/// volumeTypeToString ///
///
/// \brief converts the passed enumerated volume type to a string
///
inline std::string volumeTypeToString(Volume_Type t)
{
    switch (t) {
        case Volume_Type::MUSIC:
            return "MUSIC";
        case Volume_Type::GAME:
            return "GAME";
        case Volume_Type::UI:
            return "UI";
        default:
            return std::string();
    }
}

/// stringToVolumeType ///
///
/// \brief converts the passed string to an enumerated volume type
inline Volume_Type stringToVolumeType(std::string s)
{
    if (s == "MUSIC") {
        return Volume_Type::MUSIC;
    }
    else if (s == "GAME") {
        return Volume_Type::GAME;
    }
    else if (s == "UI") {
        return Volume_Type::UI;
    }
    else {
        return Volume_Type::NULL_TYPE;
    }
}
