#pragma once

#include <SFML/Graphics.hpp>

#include "building.hpp"

class Lootable : public Building {
public:
    Lootable();
    void resize();
    void addItem(std::shared_ptr<Item> item);

private:
};
