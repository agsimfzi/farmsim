#pragma once

#include <SFML/Graphics.hpp>

#include <item/reactions.hpp>

class Building {
public:
    Building() = default;

    std::string name;
    size_t uid;

    int value;

    bool active = false;
    std::vector<Reaction> reactions;

    enum Type {
        FURNACE,
        NULL_TYPE
    };

    Type type;

    static inline Type stringToType(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        if (s == "FURNACE") {
            return FURNACE;
        }
        else {
            return Type::NULL_TYPE;
        }
    }
};
