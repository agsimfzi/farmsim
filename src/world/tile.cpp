#include <world/tile.hpp>

Tile_Info::Tile_Info(const Tile_Info& i)
    : coordinates { i.coordinates }
    , planted { i.planted }
    , biome { i.biome }
    , detail { std::make_unique<Detail>(*i.detail) }
    , building { i.building }
    , floor { i.floor }
    , texture_pos { i.texture_pos }
{}

void Tile_Info::setFloor(Floor_Type t)
{
    floor = t;
    texture_pos.y = static_cast<int>(t) * tile_size;
}
