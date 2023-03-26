#pragma once

#include <memory>

#include <entity/player.hpp>

#include <item/player_inventory.hpp>

#include "chunk.hpp"
#include "direction.hpp"
#include "tile_info.hpp"

/// CHUNK LOADER ///
///
/// \brief interface for world chunks which dynamically loads and unloads them as the player moves through the world
///
class Chunk_Loader {
public:
    Chunk_Loader() = delete;

/// FULL CONSTRUCTOR ///
///
/// \brief loads tile map reference
///
    Chunk_Loader(Map_Tile<Floor_Info>& info);

/// load ///
///
/// \brief loads local chunks in a 3x3 grid centered on the player
///
/// \param current season, used to select proper tree sprites
///
    void load(Season s);

/// check ///
///
/// \brief reloads chunks if the player is OOB from the center chunk
///
    void check(sf::Vector2i player_coordinates, Season s);

/// getChunks ///
///
/// \brief returns a reference to all chunks
///
    const Map_Tile<std::unique_ptr<Chunk>>& getChunks() const;

/// findChunk ///
///
/// \brief returns the index of the chunk that contains the passed tile coordinates
///
    sf::Vector2i findChunk(sf::Vector2i coords);

/// findChunk ///
///
/// \brief returns the index of the chunk that contains the passed world position
///
    sf::Vector2i findChunk(sf::Vector2f pos);

/// chunk ///
///
/// \brief returns a pointer to the indexed chunk
///
    Chunk* chunk(sf::Vector2i i);

/// currentChunk ///
///
/// \brief returns the chunk the player is currently in
///
    Chunk* currentChunk();

/// localChunks ///
///
/// \brief returns a list of pointers to local chunks in a 3x3 grid around the player
///
    std::vector<Chunk*> localChunks();

    const sf::Vector2i chunk_size{ 24, 24 }; /**< chunk size in tiles */
    sf::Vector2i world_min; /**< smallest tile index */
    sf::Vector2i world_max; /**< largest tile index */

/// clear ///
///
/// \brief erases all chunks and sets the current chunk to the origin
///
    void clear();

/// floor ///
///
/// \brief returns a pointer to the floor tile at the passed coordinates
///
    Floor* floor(sf::Vector2i i);

/// detail ///
///
/// \brief returns a pointer to the detail, if any, at the passed coordinates
///
    Detail* detail(sf::Vector2i i);

/// tree ///
///
/// \brief returns a pointer to the tree, if any, at the passed coordinates
///
    Tree* tree(sf::Vector2i i);

/// rock ///
///
/// \brief returns a pointer to the rock sprite, if any, at the passed coordinates
///
    sf::Sprite* rock(sf::Vector2i i);

/// building ///
///
/// \brief returns a pointer to the building, if any, at the passed coordinates
///
    std::shared_ptr<Building> building(sf::Vector2i i);

/// eraseDetail ///
///
/// \brief deletes the detail at the passed coordinates
///
    void eraseDetail(sf::Vector2i i);

/// eraseTree ///
///
/// \brief deletes the tree at the passed coordinates
///
    void eraseTree(sf::Vector2i i);

/// eraseRock ///
///
/// \brief deletes the rock sprite at the passed coordinates
///
    void eraseRock(sf::Vector2i i);

/// eraseBuilding ///
///
/// \brief deletes the building at the passed coordinates
///
    void eraseBuilding(sf::Vector2i i);

/// addBuilding ///
///
/// \brief places passed building at the passed coordinates
///
    void addBuilding(std::shared_ptr<Building> b, sf::Vector2i coords);

/// addItem ///
///
/// \brief drops an item in the world at the passed coordinates
///
    void addItem(std::shared_ptr<Item> item, sf::Vector2i coords);

/// checkPickup ///
///
/// \brief moves items toward the player and adds them to the inventory if applicable
///
    void checkPickup(Player_Inventory& inventory, Player& player, bool pickup_all, float deltaTime);

/// update ///
///
/// \brief updates local chunks
///
    void update();

private:
    sf::Vector2i current{ 0, 0 }; /**< chunk which holds the player */
    Map_Tile<std::unique_ptr<Chunk>> chunks; /**< all chunks */

    Map_Tile<Floor_Info>& info; /**< reference to world tile map */

/// validChunkIndex ///
///
/// \brief returns true if a chunk of the passed index exists
///
    bool validChunkIndex(sf::Vector2i i);

/// validChunkIndex ///
///
/// \brief returns true if a chunk of the passed index exists
///
    bool validChunkIndex(int x, int y);
};
