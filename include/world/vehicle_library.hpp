#pragma once

#include <map>

#include "vehicle.hpp"

#include <iostream>

class Vehicle_Library {
public:
    Vehicle_Library();

private:
    std::map<Vehicle::Type, Vehicle_Data> v;

public:
    Vehicle_Data operator() (Vehicle::Type t)
    {
        std::cout << "returning vehicle " << t << '\n';
        return v[t];
    }
};
