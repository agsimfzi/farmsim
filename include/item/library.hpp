#pragma once

#include "building_library.hpp"
#include "item_library.hpp"

#include <world/crop_library.hpp>
#include <world/detail_library.hpp>
#include <world/vehicle_library.hpp>

/// LIBRARY ///
///
/// \brief Stores prototypes for items, buildings, crops, and vehicles.
///
class Library {
public:
    Library() = default;

/// ITEM ///
///
/// \brief returns a new shared pointer to an Item, by name
///
    std::shared_ptr<Item> item(std::string name);

/// ITEM ///
///
/// \brief returns a new shared pointer to an Item, by UID
///
    std::shared_ptr<Item> item(size_t uid);

/// BUILDING ///
///
/// \brief returns a new shared pointer to a Building, by name
///
    std::shared_ptr<Building> building(std::string name);

/// CROP ///
///
/// \brief returns a new shared pointer to a building, by UID
///
    std::shared_ptr<Building> building(size_t uid);

/// CROP ///
///
/// \brief returns a Crop object by its seed's UID
///
    Crop crop(size_t uid);

/// VEHICLE ///
///
/// \brief returns a Vehicle_Data struct for constructing a Vehicle
///
    Vehicle_Data vehicle(Vehicle::Type type);

/// DETAIL ///
///
/// \brief returns a Detail object
///
    Detail detail(std::string name);

// libraries are made accessible to avoid circular dependencies
    Item_Library items; /**< stores item prototypes */
    Building_Library buildings; /**< stores building prototypes */
    Crop_Library crops; /**< stores crop prototypes */
    Vehicle_Library vehicles; /**< stores vehicle data */

    Detail_Library details; /**< stores detail prototypes */
};
