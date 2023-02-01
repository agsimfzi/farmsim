#include <world/biome_generator.hpp>

#include <math.h>
#include <vector>

#include <util/prng.hpp>

#include <world/perlin_noise.hpp>
#include <world/radial_noise.hpp>

#include <iostream>

Biome_Generator::Biome_Generator(sf::Vector2i min, sf::Vector2i max)
    : world_min{ min }
    , world_max{ max }
{
    size = world_max - world_min;
}

void Biome_Generator::clear()
{
    map.clear();
}

Map_Tile<Biome>& Biome_Generator::generate()
{
    auto seed = [&]() { return prng::number(0u, UINT_MAX); };
    Perlin_Noise perlin_biome(seed());
    Perlin_Noise perlin_biome1(seed());

    std::vector<Perlin_Noise> perlin_land;
    size_t land_count = 4;
    for (size_t p = 0; p < land_count; p++) {
        perlin_land.push_back(Perlin_Noise(seed()));
    }

    Radial_Noise radial_noise(world_min, world_max);

    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            Biome b;
            double i = 10.d * ((double)x / (double)size.x);
            double j = 10.d * ((double)y / (double)size.y);

            double o = 1.d;
            for (size_t p = 0; p < land_count; p++) {
                o *= perlin_land[p].noise(i, j);
            }

            o *= radial_noise.inv(x, y);

            if (o <= 0.01d) {
                b = Biome::OCEAN;
            }
            else {
                double t = perlin_biome.noise(i, j);
                t *= perlin_biome1.noise(i, j);
                if (t < 0.25d) {
                    b = Biome::GRASSLAND;
                }
                else {
                    b = Biome::FOREST;
                }
            }

            map[x][y] = b;
        }
    }

    return map;
}

sf::Vector2i Biome_Generator::getMin()
{
    return world_min;
}

sf::Vector2i Biome_Generator::getMax()
{
    return world_max;
}
