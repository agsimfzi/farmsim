#pragma once

#include <SFML/Graphics.hpp>

#include "building.hpp"

/// LOOTABLE ///
///
/// \brief Building specialization that may be destroyed to drop its held items.
class Lootable : public Building {
public:

/// DEFAULT CONSTRUCTOR ///
///
    Lootable();

/// FULL CONSTRUCTOR ///
///
/// \brief invokes base constructor to define animated sprite
///
    Lootable(Building_Animation_Data ad);

/// addItem ///
///
/// \brief adds an item to the lootable.
/// can be performed many times, as it expands the inventory.
///
    void addItem(std::shared_ptr<Item> item);
};
