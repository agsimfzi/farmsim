#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <item/item.hpp>
#include <item/reactions.hpp>

class Building {
public:
    Building() = default;

    std::string name;
    size_t uid;
    int value;

    bool active = false;
    std::vector<Reaction> reactions;

    std::shared_ptr<Item> active_reagant = nullptr;
    std::shared_ptr<Item> active_product = nullptr;

    enum Type {
        FURNACE,
        CONTAINER,
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

    bool interface = true;

    bool validReagant(std::string name);

    void checkReaction();
};
