#pragma once

#include <SFML/Graphics.hpp>

#include "inventory_cell.hpp"

class Building_Interface : public sf::Drawable {
public:
    Building_Interface() = default;

private:
    Inventory_Cell reagant;
    Inventory_Cell product;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
