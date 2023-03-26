#pragma once

#include <SFML/Graphics/Rect.hpp>

#include <string>

struct Detail_Data {
    std::string name;
    std::string type_string;
    std::string product;
    int quantity_min;
    int quantity_max;

    sf::IntRect texture_rect;

    short int health;
    short int max_health;
};
