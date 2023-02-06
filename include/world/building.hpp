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
    std::vector<Reaction> reactions;

    enum Type {
        CONTAINER,
        FURNACE,
        SAW,
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

    int current_reaction = -1;
    size_t reaction_tick = 0;

    void checkReaction();

    void tick(Item_Library& item_library);

    float progress();

    void endReaction();

    void clearReagant();
    void clearProduct();

    void setReagant(Item* item);
    void setProduct(Item* item);

    Item* activeReagant();
    Item* activeProduct();

private:
    std::shared_ptr<Item> active_reagant = nullptr;
    std::shared_ptr<Item> active_product = nullptr;
};
