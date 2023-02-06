#include <world/building.hpp>

#include <util/primordial.hpp>

#include <iostream>

bool Building::validReagant(std::string name)
{
    //std::cout << "testing " << name << " as valid reagant against...\n";
    for (const auto& r : reactions) {
        //std::cout << "\t" << r.reagant << "...";
        if (equalStrings(name, r.reagant)) {
            //std::cout << "TRUE!\n";
            return true;
        }
        //std::cout << '\n';
    }

    return false;
}

void Building::checkReaction()
{}
