#pragma once

#include "radial_noise.hpp"
#include "tile.hpp"
#include "world.hpp"

class Biome_Generator {
public:
    Biome_Generator(sf::Vector2i min, sf::Vector2i max);

    void clear();
    Map_Tile<Biome>& generate();

private:
    Map_Tile<Biome> biomes;
    Map_Tile<Detail_Type> details;
    Map_Tile<bool> ocean;
    Map_Tile<bool> lake;
    Map_Tile<bool> rivers;
    Map_Tile<bool> beach;
    Map_Tile<bool> empty;

    sf::Vector2i world_min;
    sf::Vector2i world_max;
    sf::Vector2i size;

    Radial_Noise radial_noise;

    bool adjacentOcean(int x, int y);
    bool adjacentLava(int x, int y);

    void runRivers();
};
