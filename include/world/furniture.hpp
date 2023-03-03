#pragma once

#include "building.hpp"

class Furniture : public Building {
public:
    Furniture() = default;
    Furniture(Building_Animation_Data ad);

private:
};
