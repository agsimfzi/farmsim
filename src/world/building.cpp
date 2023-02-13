#include <world/building.hpp>

#include <util/primordial.hpp> // for equalStrings

std::vector<std::vector<std::shared_ptr<Item>>>& Building::getInventory()
{
    return inventory;
}
