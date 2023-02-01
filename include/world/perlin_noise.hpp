#pragma once

#include <vector>

//licensed under GPL v3

class Perlin_Noise {
public:
    // Initialize with the reference values for the permutation vector
    Perlin_Noise();
    // Generate a new permutation vector based on the value of seed
    Perlin_Noise(unsigned int seed);
    // Get a noise value, for 2D images z can have any value
    double noise(double x, double y, double z = 0.8d);
private:
    // The permutation vector
    std::vector<int> p;

    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};
