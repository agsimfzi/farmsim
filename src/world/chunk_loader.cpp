#include <world/chunk_loader.hpp>

Chunk_Loader::Chunk_Loader(Map_Tile<Floor_Info>& info)
    : info { info }
{}

void Chunk_Loader::load()
{
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            sf::Vector2i i(x, y);
            i += current;
            if (!validChunkIndex(i)) {
                sf::Vector2i start(i);
                start.x *= chunk_size.x;
                start.x += world_min.x;
                start.y *= chunk_size.y;
                start.y += world_min.y;
                chunks[i.x][i.y] = std::make_unique<Chunk>(start, chunk_size, info);
            }
        }
    }
}

void Chunk_Loader::check(sf::Vector2i player_coordinates)
{
    if (!validChunkIndex(current) || !chunks[current.x][current.y]->contains(player_coordinates)) {
        sf::Vector2i new_current = findChunk(player_coordinates);

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                sf::Vector2i i(x, y);
                i += current;
                if (validChunkIndex(i)) {
                    i -= new_current;
                    if (i.x < -1 || i.x > 1 || i.y < -1 || i.y > 1) {
                        chunks[x][y] = nullptr;
                        chunks[x].erase(y);
                    }
                }
            }
        }

        current = new_current;

        load();
    }
}

const Map_Tile<std::unique_ptr<Chunk>>& Chunk_Loader::getChunks() const
{
    return chunks;
}

sf::Vector2i Chunk_Loader::findChunk(sf::Vector2i coords)
{
    coords -= world_min;
    coords.x /= chunk_size.x;
    coords.y /= chunk_size.y;
    return coords;
}

Chunk* Chunk_Loader::chunk(sf::Vector2i i)
{
    Chunk* c = nullptr;
    if (validChunkIndex(i)) {
        c = chunks[i.x][i.y].get();
    }
    return c;
}

void Chunk_Loader::clear()
{
    chunks.clear();
    current = sf::Vector2i(0, 0);
}

Floor* Chunk_Loader::floor(sf::Vector2i i)
{
    Floor* t = nullptr;
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        t = chunks[c.x][c.y]->getFloor(i);
    }
    return t;
}

Detail* Chunk_Loader::detail(sf::Vector2i i)
{
    Detail* t = nullptr;
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        t = chunks[c.x][c.y]->getDetail(i);
    }
    return t;
}

void Chunk_Loader::eraseDetail(sf::Vector2i i)
{
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->eraseDetail(i);
    }
}

bool Chunk_Loader::validChunkIndex(sf::Vector2i i)
{
    return (chunks.contains(i.x) && chunks[i.x].contains(i.y) && chunks[i.x][i.y]);
}

bool Chunk_Loader::validChunkIndex(int x, int y)
{
    return validChunkIndex(sf::Vector2i(x, y));
}

void Chunk_Loader::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (chunks.contains(current.x + x)
            && chunks.at(current.x + x).contains(current.y + y)
            && chunks.at(current.x + x).at(current.y + y)) {
                target.draw(*chunks.at(current.x + x).at(current.y + y), states);
            }
        }
    }
}
