#pragma once

#include "radial_noise.hpp"
#include "biome.hpp"
#include "world.hpp"

/// BIOME GENERATOR ///
/// \brief brief
///
class Biome_Generator {
public:

/// FULL CONSTRUCTOR ///
/// \brief brief
///
/// \param param
///
    Biome_Generator(sf::Vector2i min, sf::Vector2i max);

/// clear ///
/// \brief clears all tile maps
///
    void clear();

/// generate ///
/// \brief creates world biomes
///
/// \return return Biome tile map
///
    Map_Tile<Biome>& generate();

/// getStartCoordinates ///
/// \brief finds an appropriate starting beach for the player
///
/// \return start index
///
    sf::Vector2i getStartCoordinates();

private:
    Map_Tile<Biome> biomes; /**< stores biome data */
    Map_Tile<Detail_Type> details; /**< stores detail data */
    Map_Tile<bool> ocean; /**< stores oceans */
    Map_Tile<bool> lake; /**< stores lakes */
    Map_Tile<bool> rivers; /**< stores rivers */
    Map_Tile<bool> beach; /**< stores beaches */
    Map_Tile<bool> empty; /**< tracks empty tiles */

    std::vector<std::vector<sf::Vector2i>> beaches; /**< stores beaches as discrete contiguous regions */

    sf::Vector2i world_min; /**< lowest world tile index */
    sf::Vector2i world_max; /**< highest world tile index */
    sf::Vector2i size; /**< world size in tiles, computed from world_min and world_max */

    Radial_Noise radial_noise; /**< provides a radial gradient for noise generation */

/// adjacentOcean ///
/// \brief internal
///
/// \param param x Tile index
/// \param param y Tile index
///
/// \return return True if any adjacent tiles are ocean (diagonal included)
///
    bool adjacentOcean(int x, int y);

/// adjacentLava ///
/// \brief internal
///
/// \param param x Tile index
/// \param param y Tile index
///
/// \return return True if any adjacent tiles are lava (diagonal included)
///
    bool adjacentLava(int x, int y);

/// checkForIsland ///
/// \brief Performs a very simple test checking that the tile is connected to the central island. For setting start coordinates.
///
/// \param param i Tile index to check
///
/// \return true if connection check passes
///
    bool checkForIsland(sf::Vector2i i);

/// runRivers ///
/// \brief Spawns one river, near the center, which travels toward the ocean.
/// currently awaiting full implementation and this is inactive
///
/// \return return
///
    void runRivers();

// the following two functions must be genericized for full biome implementation

/// splitBeaches() ///
/// \brief splits the beach tile map into discrete contiguous regions
///
    void splitBeaches();

/// floodBeach ///
/// \brief performs a flood fill for splitBeaches()
///
    void floodBeach(sf::Vector2i i);
};
