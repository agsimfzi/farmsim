#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <world/building.hpp>
#include <world/container.hpp>
#include <world/crafting.hpp>
#include <world/furniture.hpp>
#include <world/lootable.hpp>
#include <world/machine.hpp>
//#include <world/structure.hpp>

/// BUILDING LIBRARY ///
///
/// \brief stores building prototypes
///
class Building_Library {
public:

/// DEFAULT CONSTRUCTOR ///
///
/// \brief reads building data from the database and constructs buildings from them, which are stored
    Building_Library();

/// OPERATOR () (SIZE_T) ///
///
/// \brief returns a new shared building pointer of the passed unique ID. includes proper construction of subclasses
///
    std::shared_ptr<Building> operator ()(size_t uid)
    {
        return makeBySubtype(uid_shelf[uid].get());
    }

/// OPERATOR () (STRING) ///
///
/// \brief returns a new shared building pointer of the passed name. includes proper construction of subclasses
///
    std::shared_ptr<Building> operator ()(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        return makeBySubtype(string_shelf[name].get());
    }

private:
    std::map<std::string, std::shared_ptr<Building>> string_shelf; /**< stores buildings by name */
    std::map<size_t, std::shared_ptr<Building>> uid_shelf; /**< stores buildings by unique ID */

/// makeBySubtype ///
///
/// \brief creates a new shared pointer of a building's derived type, so re-casting is possible to access non-polymorphic derived behavior
///
    std::shared_ptr<Building> makeBySubtype(Building* b);

/// findBuildingType ///
///
/// \brief reads a building's type using the passed subtype string
///
    Building::Type findBuildingType(std::string subtype);

/// generateSeedReaction ///
///
/// \brief creates and returns a Reaction struct for obtaining seeds from a plant
///
    Reaction generateSeedReaction(Item_Data item);

/// generateWoodReaction ///
///
/// \brief creates and returns a Reaction struct for turning logs into planks
///
    Reaction generateWoodReaction(Item_Data item);
};
