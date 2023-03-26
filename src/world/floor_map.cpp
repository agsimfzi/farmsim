#include <world/floor_map.hpp>

#include <resources/texture_manager.hpp>

#include <world/tile_size.hpp>

#include <util/primordial.hpp>

Floor_Map::Floor_Map()
{
    texture = &Texture_Manager::get("FLOOR");
    tsize.x = tile_size;
    tsize.y = tile_size;

    vertices.setPrimitiveType(sf::Quads);
}

void Floor_Map::load(Map_Tile<Tile>& floor, sf::Vector2i start, sf::Vector2i size)
{
    vertices.resize(size.x * size.y * 4);
    for (int x = start.x; x < start.x + size.x; x++) {
        for (int y = start.y; y < start.y + size.y; y++) {
            sf::Vertex* quad = &vertices[(x - start.x + (y - start.y) * size.x) * 4];

            quad[0].position = sf::Vector2f((x - start.x) * tsize.x, (y - start.y) * tsize.y);
            quad[1].position = sf::Vector2f((x - start.x + 1) * tsize.x, (y - start.y) * tsize.y);
            quad[2].position = sf::Vector2f((x - start.x + 1) * tsize.x, (y - start.y + 1) * tsize.y);
            quad[3].position = sf::Vector2f((x - start.x) * tsize.x, (y - start.y + 1) * tsize.y);

            sf::Vector2f tc(floor[x][y].texture_pos);

            quad[0].texCoords = tc;
            quad[1].texCoords = tc + sf::Vector2f(tile_size, 0.f);
            quad[2].texCoords = tc + sf::Vector2f(tile_size, tile_size);
            quad[3].texCoords = tc + sf::Vector2f(tile_size, tile_size);
        }
    }

    setPosition(sf::Vector2f(start) * tile_size);
}

void Floor_Map::change(size_t index, sf::Vector2i texture_pos, sf::Vector2i size)
{
    sf::Vertex* quad = &vertices[index * 4];

    sf::Vector2f tc(texture_pos);

    quad[0].texCoords = tc;
    quad[1].texCoords = tc + sf::Vector2f(tile_size, 0.f);
    quad[2].texCoords = tc + sf::Vector2f(tile_size, tile_size);
    quad[3].texCoords = tc + sf::Vector2f(tile_size, tile_size);
}

void Floor_Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = texture;
    target.draw(vertices, states);
}
