#pragma once

#include "building.hpp"

class Container : public Building {
public:
    Container();

    void setItem(std::shared_ptr<Item> item, sf::Vector2i i);
    void clearItem(sf::Vector2i i);
};
