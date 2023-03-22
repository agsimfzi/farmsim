#pragma once

#include <vector>

//licensed under GPL v3

/// PERLIN NOISE ///
///
/// \brief noise generator, found at [url].
/// this file has been edited for doxygen.
///
class Perlin_Noise {
public:

/// DEFAULT CONSTRUCTOR ///
///
/// \brief Initialize with the reference values for the permutation vector
///
    Perlin_Noise();

/// SEEDED CONSTRUCTOR ///
///
/// \brief Generate a new permutation vector based on the value of seed
///
    Perlin_Noise(unsigned int seed);

/// noise ///
///
/// \brief Get a noise value, for 2D images z can have any value
///
    double noise(double x, double y, double z = 0.8d);

private:
    std::vector<int> p; /**< The permutation vector */

    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};
