#include <world/world.hpp>

#include <cmath>
#include <iostream>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <util/primordial.hpp>
#include <util/prng.hpp>
#include <util/vector2_stream.hpp>

//////////////////////////////////////////////////////////////

const sf::Vector2i World::renderDistance { 64, 64 };

World::World(Item_Library& item_library)
    : item_library { item_library }
    , textureFloors { Texture_Manager::get("FLOOR") }
    , textureWalls { Texture_Manager::get("WALL") }
    , textureDetails { Texture_Manager::get("DETAILS") }
    , textureTiledDetail { Texture_Manager::get("TILING") }
{
}

void World::reset()
{
    floorMap.clear();
    walls.clear();
    details.clear();
    for (auto& c : crops) {
        c.second.clear();
    }
    crops.clear();
}

void World::update(Player_Inventory& inventory)
{
    if (tickClock.getElapsedTime().asSeconds() >= 0.1f) {
        tickClock.restart();
        tick();
    }

    if (interacting) {
        interact(inventory);
    }
}

void World::interact(Player_Inventory& inventory)
{
    if (player_target.active) {
        sf::Vector2i t = player_target.coordinates;
        Floor* f = floor[t.x][t.y].get();
        if (f->planted) {
            if (!crops[t.x].contains(t.y)) {
                std::cout << "FAILED TO FIND CROP AT TILE " << t << '\n';
            }
            else if (crops[t.x][t.y].fullyGrown()) {
                inventory.addItem(item_library.item(crops[t.x][t.y].getUID()));
                std::map<int, Crop>::iterator it = crops[t.x].find(t.y);
                crops[t.x].erase(it);
                f->setType(Floor_Type::TILLED);
                f->planted = false;
            }
        }
    }
}

void World::checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords)
{
    // future: pass player coordinates + tool distance
    mpos += sf::Vector2f(sign(mpos.x) * (Tile::tileSize / 2.f), sign(mpos.y) * (Tile::tileSize / 2.f));
    sf::Vector2i coords;
    coords.x = mpos.x / Tile::tileSize;
    coords.y = mpos.y / Tile::tileSize;

    // check for tile at coordinates
    if (floor.contains(coords.x) && floor[coords.x].contains(coords.y)
        && inRange(coords, playerCoords)) {
        player_target.setActive(true);
        player_target.place(coords);
    }
    else {
        player_target.setActive(false);
    }
}

bool World::inRange(sf::Vector2i c1, sf::Vector2i c2)
{
    sf::Vector2i diff = c1 - c2;

    return ((diff.x >= -1 && diff.x <= 1)
        && diff.y >= -1 && diff.y <= 1);
}

void World::makeFloor()
{
    for (int x = worldMin.x; x <= worldMax.x; x++) {
        for (int y = worldMin.y; y <= worldMax.y; y++) {
            floor[x][y] = std::make_unique<Floor>(Floor(sf::Vector2i(x, y), textureFloors));
            floor[x][y]->setType(Floor_Type::DIRT);
        }
    }

    // place water

    // then place grass on dirt
    for (int x = worldMin.x; x <= worldMax.x; x++) {
        for (int y = worldMin.y; y <= worldMax.y; y++) {
            if (floor[x][y] && floor[x][y]->type == Floor_Type::DIRT && prng::boolean()) {
                floor[x][y]->details.push_back(Detail(Detail_Type::GRASS, Texture_Manager::get("GRASS")));
                floor[x][y]->details.back().setPosition(floor[x][y]->getPosition());
            }
        }
    }

    updateAutotiledDetails(worldMin, worldMax);

    // add additional details here based on detail type (or tile type if there is no detail yet)

    std::cout << "\n\tfloor is made!";
}

void World::updateAutotiledDetails(sf::Vector2i start, sf::Vector2i end)
{
    for (int x = start.x; x <= end.x; x++) {
        for (int y = start.y; y <= end.y; y++) {
            Floor* f = floor[x][y].get();
            if (f && f->details.size() > 0) {
                Detail* d;
                if (f->details.front().type == Detail_Type::GRASS) {
                    d = &f->details.front();
                }
                else {
                    std::cout << "\n\nCRITICAL ERROR: something is seriously wrong in World::updateAutotiledDetails!!!!!\n\n";
                }
                f->details[0];
                sf::Vector2i pos(autotileX(sf::Vector2i(x, y), d->type), 0);
                sf::Vector2i size(Tile::tileSize, Tile::tileSize);
                d->setTextureRect(sf::IntRect(pos, size));
                d->setOrigin(sf::Vector2f(size / 2));
            }
        }
    }
}

int World::autotileX(sf::Vector2i i, Detail_Type type)
{
    bool n = adjacentDetailMatch(i + sf::Vector2i(0, -1), type);
    bool w = adjacentDetailMatch(i + sf::Vector2i(-1, 0), type);
    bool s = adjacentDetailMatch(i + sf::Vector2i(0, 1), type);
    bool e = adjacentDetailMatch(i + sf::Vector2i(1, 0), type);

    int sum = 0;
    if (n) {
        sum += 1;
    }
    if (w) {
        sum += 2;
    }
    if (s) {
        sum += 4;
    }
    if (e) {
        sum += 8;
    }
    return (sum * roundFloat(Tile::tileSize));
}

bool World::adjacentDetailMatch(sf::Vector2i i, Detail_Type type)
{
    return (floor[i.x][i.y] && floor[i.x][i.y]->details.size() > 0 && floor[i.x][i.y]->details.back().type == type);
}

void World::makeWalls()
{
    std::cout << "\nmaking walls...";
    std::cout << "\n\twalls made!";
}

void World::makeDetails()
{
}

bool World::hasOrthogonalFloor(sf::Vector2i v)
{
    return (floorMap[v.x - 1][v.y]
        || floorMap[v.x + 1][v.y]
        || floorMap[v.x][v.y - 1]
        || floorMap[v.x][v.y + 1]);
}

bool World::hasDiagonalFloor(sf::Vector2i v)
{
    return (floorMap[v.x - 1][v.y - 1]
        || floorMap[v.x + 1][v.y - 1]
        || floorMap[v.x - 1][v.y + 1]
        || floorMap[v.x + 1][v.y + 1]);
}

Map_Tile<Floor>& World::getFloor()
{
    return floor;
}

Map_Tile<Wall>& World::getWalls()
{
    return walls;
}

Tile* World::getWall(int x, int y)
{
    if (walls.contains(x) && walls[x].contains(y)) {
        return walls[x][y].get();
    }
    else if (doors.contains(x) && doors[x].contains(y)) {
        return doors[x][y].get();
    }
    else {
        return nullptr;
    }
}

void World::erase()
{
    floor.clear();
    walls.clear();
    details.clear();
    doors.clear();
}

std::vector<sf::FloatRect> World::getLocalWalls(sf::Vector2i p)
{
    std::vector<sf::FloatRect> cWalls;

    const static int depth = 1;

    for (int x = p.x - depth; x <= p.x + depth; ++x) {
        for (int y = p.y - depth; y <= p.y + depth; ++y) {
            Tile* wall = getWall(x, y);
            if (wall != nullptr) {
                cWalls.push_back(wall->getGlobalBounds());
            }
        }
    }

    return cWalls;
}

std::vector<sf::FloatRect> World::getLocalWalls(sf::Vector2f p)
{
    return getLocalWalls(sf::Vector2i(p / Tile::tileSize));
}

void World::useItem(Item* item)
{
    switch (item->getType()) {
        case Item_Type::TOOL:
            useTool(item);
            break;
        case Item_Type::SEED:
            plantCrop(item);
            break;
        default:
            break;
    }
}

void World::useTool(Item* item)
{
    switch (item->getUID()) {
        case 0:
            hoe();
            break;
        case 1:
            water();
            break;
        default:
            break;
    }
}

void World::hoe()
{
    if (player_target.active) {
        sf::Vector2i t = player_target.coordinates;
        if (floor[t.x][t.y]->type == Floor_Type::DIRT) {
            if (floor[t.x][t.y]->details.size() > 0 && floor[t.x][t.y]->details.front().type == Detail_Type::GRASS) {
                floor[t.x][t.y]->details.pop_front();
                updateAutotiledDetails(t - sf::Vector2i(1, 1), t + sf::Vector2i(1, 1));
            }
            else {
                floor[t.x][t.y]->setType(Floor_Type::TILLED);
            }
        }
    }
}

void World::water()
{
    changeActiveTile(Floor_Type::TILLED, Floor_Type::WATERED);
}

void World::plantCrop(Item* item)
{
    if (player_target.active) {
        sf::Vector2i t = player_target.coordinates;
        Floor* f = floor[t.x][t.y].get();
        if ((f->type == Floor_Type::TILLED
                || f->type == Floor_Type::WATERED)
            && !f->planted) {
            Crop_Data d;
            d.uid = item->getUID() + 1000;
            d.coordinates = t;
            d.growth_coef = 0.01f;

            sf::Sprite sprite;
            std::string texture = "CROPS" + std::to_string((item->getUID() % 1000) / 100);
            sprite.setTexture(Texture_Manager::get(texture));
            sf::Vector2i pos(0, (item->getUID() % 100) * 64);
            sf::Vector2i size(64, 64);
            sprite.setTextureRect(sf::IntRect(pos, size));
            sprite.setOrigin(sf::Vector2f(size) / 2.f);
            sprite.setPosition(sf::Vector2f(t) * Tile::tileSize);

            item->take(1);
            f->planted = true;

            crops[t.x][t.y] = Crop(d);
            crops[t.x][t.y].setSprite(sprite);
        }
    }
}

bool World::changeActiveTile(Floor_Type prereq, Floor_Type ntype)
{
    bool change = false;

    if (player_target.active) {
        sf::Vector2i t = player_target.coordinates;
        change = (floor[t.x][t.y]->type == prereq);
        if (change) {
            floor[t.x][t.y]->setType(ntype);
        }
    }

    return change;
}

void World::tick()
{
    for (auto& x : crops) {
        for (auto& y : x.second) {
            sf::Vector2i c = y.second.getCoordinates();
            y.second.tick(floor[c.x][c.y]->type == Floor_Type::WATERED);
        }
    }
}

void World::setInteracting(bool interacting)
{
    this->interacting = interacting;
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2i w(target.getView().getCenter() / Tile::tileSize);

    for (int x = w.x - renderDistance.x; x <= w.x + renderDistance.x; ++x) {
        for (int y = w.y - renderDistance.y; y <= w.y + renderDistance.y; ++y) {
            if (floor.count(x) && floor.at(x).count(y) && floor.at(x).at(y) != nullptr) {
                target.draw(*floor.at(x).at(y), states);
                for (const auto& detail : floor.at(x).at(y)->details) {
                    target.draw(detail, states);
                }
            }
            else if (walls.count(x) && walls.at(x).count(y) && walls.at(x).at(y) != nullptr) {
                target.draw(*walls.at(x).at(y), states);
            }
        }
    }

    for (const auto& x : crops) {
        for (const auto& y : x.second) {
            target.draw(y.second, states);
        }
    }

    target.draw(player_target, states);
}
