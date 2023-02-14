#pragma once

#include <vector>

struct Reaction {
    std::string name;
    std::vector<std::string> reagants;
    std::vector<short unsigned int> reagant_count;
    std::string product;
    size_t length;
};
