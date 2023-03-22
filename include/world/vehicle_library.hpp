#pragma once

#include <map>

#include "vehicle.hpp"

/// VEHICLE LIBRARY ///
///
/// \brief Stores vehicle data by type
///
class Vehicle_Library {
public:
/// FULL CONSTRUCTOR ///
///
/// \brief retrieves vehicle data from the database and stores it
///
    Vehicle_Library();

private:
    std::map<Vehicle::Type, Vehicle_Data> v; /**< vehicle data storage */

public:
/// operator () ///
///
/// \brief retrieves vehicle data by type
    Vehicle_Data operator () (Vehicle::Type type)
    {
        return v[type];
    }
};
