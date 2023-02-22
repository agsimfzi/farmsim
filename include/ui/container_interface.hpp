#pragma once

#include <SFML/Graphics.hpp>

#include "inventory_interface.hpp"

class Container_Interface : public Inventory_Interface {
public:
    Container_Interface(Player_Inventory& inventory, sf::View& view, Container* container);

    virtual void shiftClickLeft() override;
    virtual void shiftClickRight() override;

private:
    Container* container;
    std::vector<std::vector<std::shared_ptr<Item>>>& c_inventory;

    virtual void readExtension() override;
    virtual void writeExtension() override;
};
