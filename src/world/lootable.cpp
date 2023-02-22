#include <world/lootable.hpp>

Lootable::Lootable()
{
    inventory.resize(1);
    inventory.back().resize(1);
}

void Lootable::resize()
{
    inventory.resize(1);
}

void Lootable::addItem(std::shared_ptr<Item> item)
{
    inventory.front().push_back(item);
}
