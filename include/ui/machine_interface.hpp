#pragma once

#include <SFML/Graphics.hpp>

#include "inventory_interface.hpp"

class Machine_Interface : public Inventory_Interface {
public:
    Machine_Interface(Player_Inventory& inventory, sf::View& view, Machine* machine);

    virtual void update(sf::RenderWindow& window) override;

    virtual void startDrag() override;
    virtual void placeMergeSwap() override;
    virtual void cancelDrag() override;

    virtual void clickLeft(sf::RenderWindow& window) override;

    void setProgressBarSize();

private:
    Machine* machine;

    virtual void readExtension() override;
    virtual void writeExtension() override;
    virtual void swap() override;
    void checkReaction();
};
