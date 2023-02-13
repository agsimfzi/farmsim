#pragma once

#include "building.hpp"

enum class Machine_Type {
    FURNACE,
    SAW,
    NULL_TYPE
};

inline Machine_Type stringToMachineType(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "FURNACE") {
        return Machine_Type::FURNACE;
    }
    else if (s == "SAW") {
        return Machine_Type::SAW;
    }
    else {
        return Machine_Type::NULL_TYPE;
    }
}

class Machine : public Building {
public:
    Machine();

    Machine_Type machine_type;

    bool validReagant(std::string name);

    int current_reaction = -1;
    size_t reaction_tick = 0;

    void checkReaction();

    void tick(Item_Library& item_library);

    float progress();

    void endReaction();

    void clearReagant();
    void clearProduct();

    void addReagant(Item* item);
    void setProduct(Item* item);

    std::vector<Item*> activeReagants();
    Item* activeProduct();

    bool checkReagants();
};
