#include <world/building.hpp>

#include <util/primordial.hpp> // for equalStrings

std::vector<std::vector<std::shared_ptr<Item>>>& Building::getInventory()
{
    return inventory;
}

bool Building::empty()
{
    for (const auto& r : inventory) {
        for (const auto& c : r) {
            if (c) {
                return false;
            }
        }
    }

    return true;
}
