#pragma once

#include "building.hpp"

/// FURNITURE ///
///
/// \brief specialization of Building for furniture (e.g. beds, chairs, tables)
///
class Furniture : public Building {
public:
    Furniture() = default;

/// FULL CONSTRUCTOR ///.
///
/// \brief invokes base constructor for animated sprite definition
///
    Furniture(Building_Animation_Data ad);
};
