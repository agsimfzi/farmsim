#pragma once

#include <memory>

#include "chunk.hpp"
#include "direction.hpp"
#include "tile_info.hpp"

class Chunk_Loader : public sf::Drawable {
public:
    Chunk_Loader() = delete;
    Chunk_Loader(Map_Tile<Floor_Info>& info);

    void load();

    void check(sf::Vector2i player_coordinates);

    const Map_Tile<std::unique_ptr<Chunk>>& getChunks() const;
    sf::Vector2i findChunk(sf::Vector2i coords);
    Chunk* chunk(sf::Vector2i i);

    const sf::Vector2i chunk_size{ 16, 16 };
    sf::Vector2i world_min;
    sf::Vector2i world_max;

    void clear();

    Floor* floor(sf::Vector2i i);
    Detail* detail(sf::Vector2i i);
    Tree* tree(sf::Vector2i i);

    void eraseDetail(sf::Vector2i i);
    void eraseTree(sf::Vector2i i);

private:
    void moveChunks(Direction d);
    sf::Vector2i current{ 0, 0 };
    Map_Tile<std::unique_ptr<Chunk>> chunks;

    Map_Tile<Floor_Info>& info;

    bool validChunkIndex(sf::Vector2i i);
    bool validChunkIndex(int x, int y);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
