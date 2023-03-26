#include <world/tile.hpp>

Tile::Tile(const Tile& i)
    : coordinates { i.coordinates }
    , planted { i.planted }
    , biome { i.biome }
    , detail { std::make_unique<Detail>(*i.detail) }
    , building { i.building }
    , floor { i.floor }
    , texture_pos { i.texture_pos }
    , bounds { i.bounds }
{}

void Tile::setFloor(Floor_Type t)
{
    floor = t;
    texture_pos.y = static_cast<int>(t) * tile_size;
}
