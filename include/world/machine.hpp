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

    bool validReagant(std::string name, int rxn = -1);

    Machine_Type machine_type;

    int current_reaction = -1;
    size_t reaction_tick = 0;

    void checkReaction();

    void tick(Item_Library& item_library);

    float reactionProgress();

    void endReaction();

    void clearReagant(size_t i);
    void clearProduct();

    size_t addReagant(std::shared_ptr<Item> i);
    void setProduct(std::shared_ptr<Item> item);

    std::vector<std::shared_ptr<Item>> activeReagants();
    std::shared_ptr<Item> activeProduct();

    bool checkReagants(size_t i);

private:
    std::vector<std::string> valid_reagants;

    std::shared_ptr<Item> active_product;
};
