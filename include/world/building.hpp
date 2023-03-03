#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <animation/animated_sprite.hpp>

#include <item/item.hpp>
#include <item/item_library.hpp>
#include <item/reaction.hpp>

#include "building_data.hpp"

class Building {
public:
    Building() = default;
    virtual ~Building() = default;

    Building(Building_Animation_Data ad);

    std::string name;
    size_t uid;
    int value;

    bool destructible = true;

    enum Type {
        CONTAINER,
        CRAFTING,
        FURNITURE,
        MACHINE,
        LOOTABLE,
        STRUCTURE,
        NULL_TYPE
    };

    Animated_Sprite<Building_State> sprite;

    Type type;
    std::string subtype;

    std::vector<Reaction> reactions;

    std::vector<std::vector<std::shared_ptr<Item>>>& getInventory();

    bool empty();

    short int health = 30;

    static inline Type stringToType(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        if (s == "CONTAINER") {
            return CONTAINER;
        }
        else if (s == "CRAFTING") {
            return CRAFTING;
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

    static inline std::string typeToString(Type t)
    {
        switch(t) {
            case CONTAINER:
                return "CONTAINER";
                break;
            case CRAFTING:
                return "CRAFTING";
                break;
            case FURNITURE:
                return "FURNITURE";
                break;
            case MACHINE:
                return "MACHINE";
                break;
            case LOOTABLE:
                return "LOOTABLE";
                break;
            case STRUCTURE:
                return "STRUCTURE";
                break;
            default:
                return "";
                break;
        }
    }

    bool interface = false;

    std::vector<std::vector<std::shared_ptr<Item>>> inventory;

    void updateSprite();
};
