#include <world/biome_generator.hpp>

#include <future>
#include <math.h>
#include <thread>
#include <vector>

#include <util/primordial.hpp>
#include <util/prng.hpp>

#include <world/flood_fill.hpp>
#include <world/perlin_noise.hpp>

Biome_Generator::Biome_Generator(sf::Vector2i min, sf::Vector2i max)
    : world_min{ min }
    , world_max{ max }
    , radial_noise{ min, max }
{
    size = world_max - world_min;
}

void Biome_Generator::clear()
{
    biomes.clear();
    details.clear();
    ocean.clear();
    lake.clear();
    rivers.clear();
    beach.clear();
    empty.clear();
}

Map_Tile<Biome>& Biome_Generator::generate()
{
    auto seed = []() { return prng::number(0u, UINT_MAX); };
    Perlin_Noise perlin_biome(seed());
    Perlin_Noise perlin_biome1(seed());

    std::vector<Perlin_Noise> perlin_volcano;
    size_t volcano_count = 3;
    for (size_t p = 0; p < volcano_count; p++) {
        perlin_volcano.push_back(Perlin_Noise(seed()));
    }

    std::vector<Perlin_Noise> perlin_land;
    size_t land_count = 3;
    for (size_t p = 0; p < land_count; p++) {
        perlin_land.push_back(Perlin_Noise(seed()));
    }

    //Map_Tile<bool> ocean;

    std::vector<Perlin_Noise> perlin_lake;
    size_t lake_count = 3;
    for (size_t p = 0; p < lake_count; p++) {
        perlin_lake.push_back(Perlin_Noise(seed()));
    }

    std::vector<Perlin_Noise> perlin_beach;
    size_t beach_count = 3;
    for (size_t p = 0; p < beach_count; p++) {
        perlin_beach.push_back(Perlin_Noise(seed()));
    }

    double beach_threshold = 0.001d;
    double ocean_threshold = 0.032d;
    double lake_threshold = 0.015d;
    double volcano_threshold = 0.0032d;
    double caldera_threshold = 0.02d;

    // todo: find a way to scale thresholds based on the number of noise layers



    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            double radial = radial_noise.get(x, y);
            double radial_inverse = radial_noise.inv(x, y);
            Biome b;
            double i = 10.d * ((double)x / (double)size.x);
            double j = 10.d * ((double)y / (double)size.y);

            // MAKE OCEANS
            double o = 1.d;
            for (size_t p = 0; p < land_count; p++) {
                o *= perlin_land[p].noise(i, j);
            }
            o *= radial_inverse;

            if (o <= ocean_threshold) {
                if (radial_inverse < 0.0002d)
                {
                    empty[x][y] = true;
                }
                else {
                    empty[x][y] = false;
                }
                ocean[x][y] = true;
            }
            else {
                empty[x][y] = false;
                ocean[x][y] = false;
            }

            // MAKE BEACHES FROM OCEAN NOISE

            for (size_t p = 0; p < beach_count; p++) {
                o *= perlin_beach[p].noise(i, j);
            }

            o *= radial_inverse; // apply radial gradient again

            if (ocean[x][y] || o <= beach_threshold) {
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

            if (l <= lake_threshold && radial < 0.5d) {
                lake[x][y] = true;
            }
            else {
                lake[x][y] = false;
            }


            // MAKE LAND BIOMES
            double t = perlin_biome.noise(i, j);
            t *= perlin_biome1.noise(i, j);

            double v = 1.d;
            for (size_t p = 0; p < volcano_count; p++) {
                v *= perlin_volcano[o].noise(i, j);
            }
            v *= radial;
            v *= radial;
            if (v < volcano_threshold) {
                if (radial < caldera_threshold) {
                    b = Biome::CALDERA;
                }
                else {
                    b = Biome::VOLCANO;
                }
            }
            else if (t < 0.25d) {
                b = Biome::GRASSLAND;
            }
            else {
                b = Biome::FOREST;
            }

            biomes[x][y] = b;
        }
    }

    // beach flood happens on a separate thread so it may be run concurrently with the ocean flood
    // it should take longer than the ocean flood,

    //std::packaged_task<void()> floodBeach([&]() { floodCheck(beach); });
    //std::future<void> omen = floodBeach.get_future();
    //std::thread thread = std::thread(std::move(floodBeach));
/*
    auto omen = std::async(std::launch::async, [&]() { floodCheck(beach); });

    floodCheck(ocean);

    bool joining = true;

    do {
        joining = (omen.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready);
    } while (joining);*/

    std::cout << "starting floods...\n";
    floodCheck(ocean);
    std::cout << "ocean flooded!\n";
    floodCheck(beach);
    std::cout << "beach flooded!\n";

    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            if (lake[x][y]) {
                if (adjacentLava(x, y)) {
                    lake[x][y] = false;
                    biomes[x][y] = Biome::VOLCANO;
                }
                else if (adjacentOcean(x, y)) {
                    //lake[x][y] = false;
                    //ocean[x][y] = true;
                }
            }
        }
    }

    runRivers();

    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            if (lake[x][y]) {
                biomes[x][y] = Biome::LAKE;
            }
            else if(empty[x][y]) {
                biomes[x][y] = Biome::NULL_TYPE;
            }
            else if (ocean[x][y]) {
                biomes[x][y] = Biome::OCEAN;
            }
            else if (rivers[x][y]) {
                biomes[x][y] = Biome::RIVER;
            }
            else if (beach[x][y]) {
                biomes[x][y] = Biome::BEACH;
            }
        }
    }

    return biomes;
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

bool Biome_Generator::adjacentLava(int x, int y)
{
    for (int ix = x - 1; ix <= x + 1; ix++) {
        for (int iy = y - 1; iy <= y + 1; iy++) {
            if (biomes[ix][iy] == Biome::CALDERA) {
                return true;
            }
        }
    }

    return false;
}

void Biome_Generator::runRivers()
{
    //const size_t count = prng::number(2, 5);

    //for (size_t i = 0; i < count; i++) {
        sf::Vector2i index;
        index.x = prng::number(-128, 128);
        index.y = prng::number(-128, 128);
        /*
        do {
            index.x = prng::number((world_min.x * 2) / 3, (world_max.x * 2) / 3);
            index.y = prng::number((world_min.y * 2) / 3, (world_max.y * 2) / 3);
            std::cout << "attempting to place river start at " << index << "; noise is " << radial_noise.get(index.x, index.y) << '\n';
        } while (radial_noise.get(index.x, index.y) > 0.9d);
            std::cout << "placing river start at " << index << '\n';
            */
        sf::Vector2i bias = normalizeVector(index);

        bool running = true;

        int size = 6;

        while (running) {
            for (int x = index.x - size; x <= index.x + size; x++) {
                for (int y = index.y - size; y <= index.y + size; y++) {
                    if (!ocean[x][y] && !lake[x][y]) {
                        rivers[x][y] = true;
                    }
                }
            }
            index += bias;
            if (ocean[index.x][index.y]) {
                running = false;
            }
            size = prng::number(1, 3);
        }
    //}
}
