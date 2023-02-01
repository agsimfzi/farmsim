#pragma once

#include "world.hpp"

class Radial_Noise{
public:
    Radial_Noise(sf::Vector2i min, sf::Vector2i max, sf::Vector2i center = sf::Vector2i(0, 0));
    double get(int x, int y);
    double inv(int x, int y);

private:
    sf::Vector2i min;
    sf::Vector2i max;
    sf::Vector2i size;
    sf::Vector2i center;

    Map_Tile<double> map;
};
