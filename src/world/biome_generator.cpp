#include <world/biome_generator.hpp>

#include <math.h>
#include <vector>

#include <util/prng.hpp>

#include <world/flood_fill.hpp>
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
    size_t land_count = 3;
    for (size_t p = 0; p < land_count; p++) {
        perlin_land.push_back(Perlin_Noise(seed()));
    }

    //Map_Tile<bool> ocean;
    Map_Tile<bool> lake;
    Map_Tile<bool> beach;

    std::vector<Perlin_Noise> perlin_lake;
    size_t lake_count = 3;
    for (size_t l = 0; l < lake_count; l++) {
        perlin_lake.push_back(Perlin_Noise(seed()));
    }

    Radial_Noise radial_noise(world_min, world_max);

    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            Biome b;
            if (radial_noise.inv(x, y) > 0.9d) {
                b = Biome::GRASSLAND;
            }
            else {
                double i = 10.d * ((double)x / (double)size.x);
                double j = 10.d * ((double)y / (double)size.y);

                // MAKE OCEANS
                double o = 1.d;
                for (size_t p = 0; p < land_count; p++) {
                    o *= perlin_land[p].noise(i, j);
                }
                o *= radial_noise.inv(x, y);

                double ocean_threshold = 0.02d;
                double beach_diff = 0.004d;

                if (o <= ocean_threshold) {
                    ocean[x][y] = true;
                }
                else {
                    ocean[x][y] = false;
                }

                // GENERATE BEACHES USING OCEAN NOISE AND A SLIGHTLY HIGHER THRESHOLD
                // A DISCRETE LAYER OF BEACH NOISE MAY BE USED DURING ASSIGNMENT
                // BUT DON'T USE THAT HERE, OR ELSE THE FLOOD FILL WILL FAIL :(
                if (o - beach_diff <= ocean_threshold) {
                    beach[x][y] = true;
                }
                else {
                    beach[x][y] = false;
                }


                // MAKE LAKES
                double l = 1.d;
                for (size_t p = 0; p < lake_count; p++) {
                    l *= perlin_lake[o].noise(i, j);
                }

                double lake_threshold = 0.008d;

                if (l <= lake_threshold) {
                    lake[x][y] = true;
                }
                else {
                    lake[x][y] = false;
                }


                // MAKE LAND BIOMES
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

    std::cout << "\tnoise generated!\n";

    floodCheck(ocean);
    std::cout << "\tocean flooded!\n";
    floodCheck(beach);
    std::cout << "\tbeach flooded!\n";

    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            if (!lake[x][y]) {
                if (adjacentOcean(x, y)) {
                    drainFill(x, y, lake);
                }
            }
        }
    }

    std::cout << "\tcoastal lakes drained!\n";

    Perlin_Noise b_n(seed());
    double beach_threshold = 0.3d;

    std::vector<Perlin_Noise> perlin_beach;
    size_t beach_count = 2;
    for (size_t b = 0; b < beach_count; b++) {
        perlin_beach.push_back(Perlin_Noise(seed()));
    }

    // couldn't get the beach noise to work for some reason
    // it made it all checkered... look for solutions from within

    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            if (lake[x][y]) {
                map[x][y] = Biome::LAKE;
            }
            else if (ocean[x][y]) {
                map[x][y] = Biome::OCEAN;
            }
            else if (beach[x][y]) {
                map[x][y] = Biome::BEACH;
            }
        }
    }

    std::cout << "\twater set!\n";

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

bool Biome_Generator::adjacentOcean(int x, int y)
{
    for (int ix = x - 1; ix <= x + 1; ix++) {
        for (int iy = y - 1; iy <= y + 1; iy++) {
            if (ocean[ix][iy]) {
                return true;
            }
        }
    }

    return false;
}
