#pragma once

#include "tile.hpp"
#include "world.hpp"

class Biome_Generator {
public:
    Biome_Generator(sf::Vector2i min, sf::Vector2i max);

    void clear();
    Map_Tile<Biome>& generate();

    sf::Vector2i world_min;
    sf::Vector2i world_max;
    sf::Vector2i size;

    sf::Vector2i getMin();
    sf::Vector2i getMax();

private:
    Map_Tile<Biome> map;
    Map_Tile<Detail_Type> details;
    Map_Tile<bool> ocean;
    Map_Tile<bool> empty;

    bool adjacentOcean(int x, int y);
};
