#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <world/building.hpp>
#include <world/container.hpp>
#include <world/crafting.hpp>
#include <world/furniture.hpp>
#include <world/machine.hpp>
#include <world/lootable.hpp>
//#include <world/structure.hpp>

#include <iostream>

class Building_Library {
public:
    Building_Library();
    std::shared_ptr<Building> operator ()(size_t uid) { return makeBySubtype(uidShelf[uid].get()); }
    std::shared_ptr<Building> operator ()(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (stringShelf.contains(name)) {
            std::cout << "returning " << stringShelf[name]->name << '\n';
            return makeBySubtype(stringShelf[name].get());
            //return std::make_shared<Building>(*stringShelf[name]);
        }

        std::cout << "FAILED TO FIND BUILDING OF NAME " << name << '\n';
        return nullptr;
    }

private:
    std::map<std::string, std::shared_ptr<Building>> stringShelf;
    std::map<size_t, std::shared_ptr<Building>> uidShelf;

    std::shared_ptr<Building> makeBySubtype(Building* b);

    Building::Type findBuildingType(std::string subtype);

    Reaction generateSeedReaction(Item_Data item);
};
