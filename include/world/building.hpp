#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <item/item.hpp>
#include <item/item_library.hpp>
#include <item/reaction.hpp>

class Building {
public:
    Building() = default;

    std::string name;
    size_t uid;
    int value;

    //bool active = false;

    enum Type {
        CONTAINER,
        FURNITURE,
        MACHINE,
        LOOTABLE,
        STRUCTURE,
        NULL_TYPE
    };

    Type type;

    std::vector<Reaction> reactions;

    static inline Type stringToType(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        if (s == "CONTAINER") {
            return CONTAINER;
        }
        else if (s == "FURNITURE") {
            return FURNITURE;
        }
        else if (s == "MACHINE") {
            return MACHINE;
        }
        else if (s == "LOOTABLE") {
            return LOOTABLE;
        }
        else if (s == "STRUCTURE") {
            return STRUCTURE;
        }
        else {
            return Type::NULL_TYPE;
        }
    }

    bool interface = false;

    std::vector<std::vector<std::shared_ptr<Item>>> inventory;
};
