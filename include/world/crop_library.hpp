#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "crop.hpp"

class Crop_Library {
public:
    Crop_Library();

    Crop* get(size_t uid);

private:
    std::map<size_t, std::unique_ptr<Crop>> shelf;
};
