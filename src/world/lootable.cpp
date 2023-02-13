#include <world/lootable.hpp>

Lootable::Lootable()
{
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
    inventory.back().resize(1);
}
