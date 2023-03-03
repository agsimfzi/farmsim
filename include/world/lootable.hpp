#pragma once

#include <SFML/Graphics.hpp>

#include "building.hpp"

class Lootable : public Building {
public:
    Lootable();
    Lootable(Building_Animation_Data ad);
    void addItem(std::shared_ptr<Item> item);
};
