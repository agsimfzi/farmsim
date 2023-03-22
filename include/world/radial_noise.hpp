#pragma once

#include "world.hpp"

/// RADIAL NOISE ///
///
/// \brief defines a radial gradient for influencing noise generation
class Radial_Noise{
public:

/// FULL CONSTRUCTOR ///
///
/// \brief loads boundaries and defines the size
///
    Radial_Noise(sf::Vector2i min, sf::Vector2i max, sf::Vector2i center = sf::Vector2i(0, 0));

/// get ///
///
/// \brief returns calculated gradient value for passed index
///
    double get(int x, int y);

/// inv ///
///
/// \brief returns inverse of calculated gradient value for passed index
    double inv(int x, int y);

private:
    sf::Vector2i min; /**< smallest valid tile index */
    sf::Vector2i max; /**< largest valid tile index */
    sf::Vector2i size; /**< computed world size */
    sf::Vector2i center; /**< ~center tile */
};
