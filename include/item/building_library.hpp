#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <world/building.hpp>

class Building_Library {
public:
    Building_Library();
    Building* operator ()(size_t uid) { return uidShelf[uid].get(); }
    Building* operator ()(std::string name) { return stringShelf[name].get(); }

private:
    std::map<std::string, std::shared_ptr<Building>> stringShelf;
    std::map<size_t, std::shared_ptr<Building>> uidShelf;
};
