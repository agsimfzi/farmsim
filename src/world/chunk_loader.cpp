#include <world/chunk_loader.hpp>

#include <util/primordial.hpp>

#include <util/vector2_stream.hpp>

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

        std::cout << "changing current_index from " << current << " to " << new_current << "!\n";

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

sf::Vector2i Chunk_Loader::findChunk(sf::Vector2f pos)
{
    return findChunk(sf::Vector2i(pos / Tile::tileSize));
}

Chunk* Chunk_Loader::chunk(sf::Vector2i i)
{
    Chunk* c = nullptr;
    if (validChunkIndex(i)) {
        c = chunks[i.x][i.y].get();
    }
    return c;
}

Chunk* Chunk_Loader::currentChunk()
{
    return chunks[current.x][current.y].get();
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

Tree* Chunk_Loader::tree(sf::Vector2i i)
{
    Tree* t = nullptr;
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        t = chunks[c.x][c.y]->getTree(i);
    }
    return t;
}

Rock* Chunk_Loader::rock(sf::Vector2i i)
{
    Rock* r = nullptr;
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        r = chunks[c.x][c.y]->getRock(i);
    }
    return r;
}

void Chunk_Loader::eraseDetail(sf::Vector2i i)
{
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->eraseDetail(i);
    }
}

void Chunk_Loader::eraseTree(sf::Vector2i i)
{
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->eraseTree(i);
    }
}

void Chunk_Loader::eraseRock(sf::Vector2i i)
{
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->eraseRock(i);
    }
}

void Chunk_Loader::eraseBuilding(sf::Vector2i i)
{
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->eraseBuilding(i);
    }
}

void Chunk_Loader::addBuilding(Building* b, sf::Vector2i coords)
{
    sf::Vector2i ci = findChunk(coords);
    if (chunks.contains(ci.x) && chunks[ci.x].contains(ci.y)) {
        chunks[ci.x][ci.y]->addBuilding(b, coords);
    }
}

void Chunk_Loader::addItem(std::shared_ptr<Item> item, sf::Vector2i coords)
{
    sf::Vector2i ci = findChunk(coords);
    if (chunks.contains(ci.x) && chunks[ci.x].contains(ci.y)) {
        sf::Vector2f pos = chunks[ci.x][ci.y]->getFloor(coords)->getPosition();
        chunks[ci.x][ci.y]->addItem(item, pos);
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

void Chunk_Loader::checkPickup(Player_Inventory& inventory, sf::Vector2f player_pos, bool pickup_all)
{
    const static float move_threshold = 192.f;
    const static float speed = 2.f;
    std::vector<std::pair<sf::Vector2i, std::shared_ptr<Item>>> to_move;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            sf::Vector2i ci(current.x + x, current.y + y);
            for (auto i = chunks[ci.x][ci.y]->getItems().begin();
                      i != chunks[ci.x][ci.y]->getItems().end();) {
                bool picking = (pickup_all || (*i)->can_pickup);
                if (picking) {
                    sf::Vector2f item_pos = (*i)->getPosition();

                    if ((*i)->getSprite().getGlobalBounds().contains(player_pos)) {
                        inventory.addItem((*i));
                        if (!(*i)) {
                            chunks[ci.x][ci.y]->getItems().erase(i);
                        }
                    }
                    else {
                        if (scalarDistance(player_pos, item_pos) <= move_threshold) {
                            // calculate movement vector and apply
                            sf::Vector2f offset = calculateMoveVector(item_pos, player_pos, speed);
                            (*i)->getSprite().move(offset);
                        }
                        sf::Vector2i nc = findChunk((*i)->getPosition());
                        if (ci != nc) {
                            to_move.push_back(std::make_pair(nc, *i));
                            chunks[ci.x][ci.y]->getItems().erase(i);
                        }
                        else {
                            i++;
                        }
                    }
                }
                else {
                    i++;
                }
            }
        }
    }

    for (auto& p : to_move) {
        chunks[p.first.x][p.first.y]->addItem(p.second, p.second->getSprite().getPosition());
    }
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
