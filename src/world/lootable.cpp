#include <world/lootable.hpp>

Lootable::Lootable()
{
    inventory.resize(1);
}

Lootable::Lootable(Building_Animation_Data ad)
    : Building(ad)
{
    inventory.resize(1);
}

void Lootable::addItem(std::shared_ptr<Item> item)
{
    inventory.front().push_back(item);
}
