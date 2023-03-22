#pragma once

#include <SFML/Graphics.hpp>

#include "building.hpp"

/// CRAFTING TYPE ///
///
/// \brief enumerated crafting station type
///
enum class Crafting_Type {
    WORKBENCH,
    CARPENTRY_TABLE,
    ANVIL,
    TOOL_BENCH,
    BOAT_WORKSHOP,
    MAGIC_WORKSHOP,
    NULL_TYPE
};

/// stringToCraftingType ///
///
/// \brief converts passed string to an enumerated crafting station type
inline Crafting_Type stringToCraftingType(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "WORKBENCH") {
        return Crafting_Type::WORKBENCH;
    }
    else if (s == "CARPENTRY_TABLE") {
        return Crafting_Type::CARPENTRY_TABLE;
    }
    else if (s == "ANVIL") {
        return Crafting_Type::ANVIL;
    }
    else if (s == "TOOL_BENCH") {
        return Crafting_Type::TOOL_BENCH;
    }
    else if (s == "BOAT_WORKSHOP") {
        return Crafting_Type::BOAT_WORKSHOP;
    }
    else if (s == "MAGIC_WORKSHOP") {
        return Crafting_Type::MAGIC_WORKSHOP;
    }
    else {
        return Crafting_Type::NULL_TYPE;
    }
}

/// CRAFTING ///
///
/// \brief Building specialization for crafting stations
///
class Crafting : public Building {
public:

/// DEFAULT CONSTRUCTOR ///
///
    Crafting();

/// FULL CONSTRUCTOR ///
///
/// \brief invokes base constructor with animation data
///
    Crafting(Building_Animation_Data ad);
};
