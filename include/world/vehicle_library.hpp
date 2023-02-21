#pragma once

#include <map>

#include "vehicle.hpp"

class Vehicle_Library {
public:
    Vehicle_Library();

private:
    std::map<Vehicle::Type, Vehicle_Data> v;

public:
    Vehicle_Data operator() (Vehicle::Type t)
    {
        return v[t];
    }
};
