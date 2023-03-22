#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "crop.hpp"

/// CROP LIBRARY ///
///
/// \brief stores crop prototypes
///
class Crop_Library {
public:

/// FULL CONSTRUCTOR ///
///
/// \brief retrieves crop data from the database and constructs it as crops, which are stored
///
    Crop_Library();

/// OPERATOR () ///
///
/// \brief retrieves a Crop object by uid (which matches a crop's seed)
    Crop operator ()(size_t uid)
    {
        return *shelf[uid].get();
    }

private:
    std::map<size_t, std::unique_ptr<Crop>> shelf; /**< stores crops */
};
