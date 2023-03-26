#include <world/chunk_loader.hpp>

#include <util/primordial.hpp>


Chunk_Loader::Chunk_Loader(Map_Tile<Tile_Info>& info)
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

sf::Vector2i Chunk_Loader::findChunk(sf::Vector2f pos)
{
    return findChunk(sf::Vector2i(pos / tile_size));
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

std::vector<Chunk*> Chunk_Loader::localChunks()
{
    std::vector<Chunk*> chunks;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            sf::Vector2i c(current + sf::Vector2i(x, y));
            Chunk* k = chunk(c);
            if (k) {
                chunks.push_back(k);
            }
        }
    }
    return chunks;
}

void Chunk_Loader::clear()
{
    chunks.clear();
    current = sf::Vector2i(0, 0);
}

sf::Sprite* Chunk_Loader::floor(sf::Vector2i i)
{
    sf::Sprite* f = nullptr;
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        f = chunks[c.x][c.y]->getFloor(i);
    }
    return f;
}

sf::Sprite* Chunk_Loader::detail(sf::Vector2i i)
{
    sf::Sprite* d = nullptr;
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        d = chunks[c.x][c.y]->detail(i);
    }
    return d;
}

void Chunk_Loader::updateTile(Tile_Info& info)
{
    sf::Vector2i c = findChunk(info.coordinates);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->readTile(info);
    }
}

std::shared_ptr<Building> Chunk_Loader::building(sf::Vector2i i)
{
    std::shared_ptr<Building> b = nullptr;
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        b = chunks[c.x][c.y]->getBuilding(i);
    }
    return b;
}

void Chunk_Loader::eraseDetail(sf::Vector2i i)
{
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->eraseDetail(i);
    }
}

void Chunk_Loader::eraseBuilding(sf::Vector2i i)
{
    sf::Vector2i c = findChunk(i);
    if (validChunkIndex(c)) {
        chunks[c.x][c.y]->eraseBuilding(i);
    }
}

void Chunk_Loader::addBuilding(std::shared_ptr<Building> b, sf::Vector2i coords)
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

void Chunk_Loader::checkPickup(Player_Inventory& inventory, Player& player, bool pickup_all, float deltaTime)
{
    sf::Vector2f player_pos = player.getPosition();
    const static float move_threshold = 192.f;
    const static float speed = 4.5f * deltaTime; // need to pass the delta for this
    std::vector<std::pair<sf::Vector2i, std::shared_ptr<Item>>> to_move;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            sf::Vector2i ci(current.x + x, current.y + y);
            for (auto i = chunks[ci.x][ci.y]->getItems().begin();
                      i != chunks[ci.x][ci.y]->getItems().end();) {
                bool picking = (pickup_all || (*i)->can_pickup);
                if (picking) {
                    sf::Vector2f item_pos = (*i)->getPosition();

                    // if ((*i)->getSprite().getGlobalBounds().contains(player_pos)) {
                    if (player.getBounds().contains((*i)->getSprite().getPosition())) {
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

void Chunk_Loader::update()
{
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            sf::Vector2i i(x, y);
            i += current;
            if (validChunkIndex(i)) {
                chunks[i.x][i.y]->update();
            }
        }
    }
}
