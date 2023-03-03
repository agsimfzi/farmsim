#pragma once

#include "building.hpp"

class Container : public Building {
public:
    Container();

    Container(Building_Animation_Data ad);

    std::shared_ptr<Item> getItem(sf::Vector2i i);
    void addItem(std::shared_ptr<Item>& item);
    void setItem(std::shared_ptr<Item> item, sf::Vector2i i);
    void clearItem(sf::Vector2i i);
};
