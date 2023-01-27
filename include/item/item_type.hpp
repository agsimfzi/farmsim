#pragma once

enum class Item_Type {
    CRAFTING,
    FOOD,
    TOOL,
    TREASURE,
    SEED,
    PLANT,
    NULL_TYPE
};

inline Item_Type stringToItemType(std::string s)
{
    if (s == "CRAFTING") {
        return Item_Type::CRAFTING;
    }
    else if (s == "FOOD") {
        return Item_Type::FOOD;
    }
    else if (s == "TOOL") {
        return Item_Type::TOOL;
    }
    else if (s == "TREASURE") {
        return Item_Type::TREASURE;
    }
    else if (s == "SEED") {
        return Item_Type::SEED;
    }
    else if (s == "PLANT") {
        return Item_Type::PLANT;
    }
    else {
        return Item_Type::NULL_TYPE;
    }
}
