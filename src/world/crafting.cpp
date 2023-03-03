#include <world/crafting.hpp>

Crafting::Crafting()
{
    interface = true;
}

Crafting::Crafting(Building_Animation_Data ad)
    : Building(ad)
{
    interface = true;
}
