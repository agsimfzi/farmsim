#pragma once

enum class Season {
    SPRING = 0,
    SUMMER = 1,
    AUTUMN = 2,
    WINTER = 3
};

inline std::string seasonToString(Season s)
{
    switch (s) {
        case Season::SPRING:
            return "SPRING";
        case Season::SUMMER:
            return "SUMMER";
        case Season::AUTUMN:
            return "AUTUMN";
        case Season::WINTER:
            return "WINTER";
        default:
            return std::string();
    }
}
