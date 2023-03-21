#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "crop.hpp"

class Crop_Library {
public:
    Crop_Library();

    Crop operator ()(size_t uid)
    {
        return *shelf[uid].get();
    }

private:
    std::map<size_t, std::unique_ptr<Crop>> shelf;
};
