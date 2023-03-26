#pragma once

#include <map>

#include "detail.hpp"

class Detail_Library {
public:
    Detail_Library();

    Detail operator () (std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        return shelf[name];
    }

private:
    std::map<std::string, Detail> shelf;
};
