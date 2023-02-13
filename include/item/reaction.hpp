#pragma once

#include <vector>

struct Reaction {
    std::string name;
    std::vector<std::string> reagants;
    std::string product;
    size_t length;
};
