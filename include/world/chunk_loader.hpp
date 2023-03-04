#pragma once

#include <memory>

#include <entity/player.hpp>

#include <item/player_inventory.hpp>

#include "chunk.hpp"
#include "direction.hpp"
#include "tile_info.hpp"

class Chunk_Loader {
public:
    Chunk_Loader() = delete;
    Chunk_Loader(Map_Tile<Floor_Info>& info);

    void load(Season s);

    void check(sf::Vector2i player_coordinates, Season s);

    const Map_Tile<std::unique_ptr<Chunk>>& getChunks() const;
    sf::Vector2i findChunk(sf::Vector2i coords);
    sf::Vector2i findChunk(sf::Vector2f pos);
    Chunk* chunk(sf::Vector2i i);
    Chunk* currentChunk();

    std::vector<Chunk*> localChunks();

    const sf::Vector2i chunk_size{ 24, 24 };
    sf::Vector2i world_min;
    sf::Vector2i world_max;

    void clear();

    Floor* floor(sf::Vector2i i);
    Detail* detail(sf::Vector2i i);
    Tree* tree(sf::Vector2i i);
    Rock* rock(sf::Vector2i i);
    std::shared_ptr<Building> building(sf::Vector2i i);

    void eraseDetail(sf::Vector2i i);
    void eraseTree(sf::Vector2i i);
    void eraseRock(sf::Vector2i i);
    void eraseBuilding(sf::Vector2i i);

    void addBuilding(std::shared_ptr<Building> b, sf::Vector2i coords);
    void addItem(std::shared_ptr<Item> item, sf::Vector2i coords);

    void checkPickup(Player_Inventory& inventory, Player& player, bool pickup_all, float deltaTime);

    void update();
private:
    void moveChunks(Direction d);
    sf::Vector2i current{ 0, 0 };
    Map_Tile<std::unique_ptr<Chunk>> chunks;

    Map_Tile<Floor_Info>& info;

    bool validChunkIndex(sf::Vector2i i);
    bool validChunkIndex(int x, int y);
};
