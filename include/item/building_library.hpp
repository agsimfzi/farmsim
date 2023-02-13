#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <world/building.hpp>
#include <world/container.hpp>
#include <world/crafting_table.hpp>
//#include <world/furniture.hpp>
#include <world/machine.hpp>
#include <world/lootable.hpp>
//#include <world/structure.hpp>

class Building_Library {
public:
    Building_Library();
    std::shared_ptr<Building> operator ()(size_t uid) { return uidShelf[uid]; }
    std::shared_ptr<Building> operator ()(std::string name) { return stringShelf[name]; }

private:
    std::map<std::string, std::shared_ptr<Building>> stringShelf;
    std::map<size_t, std::shared_ptr<Building>> uidShelf;

    Building::Type findBuildingType(Item_Data i);
};
