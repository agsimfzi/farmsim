#pragma once

#include <SFML/Graphics.hpp>

#include "inventory_interface.hpp"

class Machine_Interface : public Inventory_Interface {
public:
    Machine_Interface(Player_Inventory& inventory, sf::View& view, Machine* machine);

    virtual void update(sf::RenderWindow& window) override;

    virtual bool checkReactionInterface(sf::RenderWindow& window) override;

    void setProgressBarSize();

    virtual void shiftClickLeft() override;
    virtual void shiftClickRight() override;

private:
    Machine* machine;

    virtual void readExtension() override;
    virtual void writeExtension() override;
};
