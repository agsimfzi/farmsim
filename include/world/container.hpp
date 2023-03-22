#pragma once

#include "building.hpp"

/// CONTAINER ///
///
/// \brief specialized Building which contains many items, as well as a specific UI interface for manipulating its contents.
class Container : public Building {
public:

/// DEFAULT CONSTRUCTOR ///
///
/// \brief defines the inventory as 8x3
///
    Container();

/// FULL CONSTRUCTOR ///
///
/// \brief same as default constructor, but it passes the animation data to the building's base
///
/// \param ad Building_Animation_Data object
///
    Container(Building_Animation_Data ad);

/// getItem ///
///
/// \brief retrieves item from the specified cell
///
/// \return the item, or a nullptr if there is none
    std::shared_ptr<Item> getItem(sf::Vector2i i); /**< retrieves item from the specified  */

/// addItem ///
///
/// \brief attempts to add an item to the container's inventory (mirrors player_inventory logic, pending base Inventory class)
///
    void addItem(std::shared_ptr<Item>& item);

/// addItem ///
///
/// \brief places an item in the cell at the passed index, overriding any previous occupant
///
    void setItem(std::shared_ptr<Item> item, sf::Vector2i i);

/// clearItem ///
///
/// \brief erases the cell at the passed index
///
    void clearItem(sf::Vector2i i);
};
