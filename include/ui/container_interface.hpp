#pragma once

#include <SFML/Graphics.hpp>

#include "inventory_interface.hpp"

class Container_Interface : public Inventory_Interface {
public:
    Container_Interface(Player_Inventory& inventory, sf::View& view, Container* container);

private:
    Container* container;
};

