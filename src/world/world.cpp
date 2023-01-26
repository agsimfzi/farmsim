#include <world/world.hpp>

#include <cmath>
#include <iostream>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <util/primordial.hpp>
#include <util/prng.hpp>
#include <util/vector2_stream.hpp>

#include <world/automaton.hpp>

//////////////////////////////////////////////////////////////

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
    walls.clear();
    details.clear();
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
    if (activeTile) {
        sf::Vector2i t = *activeTile;
        Floor* f = floor[t.x][t.y].get();
        if (f->planted) {
            if (!crops[t.x].contains(t.y)) {
                std::cout << "FAILED TO FIND CROP AT TILE " << t << '\n';
            }
            else if (crops[t.x][t.y] && crops[t.x][t.y]->fullyGrown()) {
                inventory.addItem(item_library.item(crops[t.x][t.y]->getUID()));
                crops[t.x][t.y] = nullptr;
                f->setType(Floor_Type::TILLED);
                f->planted = false;
            }
        }
    }
}

sf::Vector2i* World::checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords)
{
    // future: pass player coordinates + tool distance
    mpos += sf::Vector2f(sign(mpos.x) * (Tile::tileSize / 2.f), sign(mpos.y) * (Tile::tileSize / 2.f));
    sf::Vector2i coords;
    coords.x = mpos.x / Tile::tileSize;
    coords.y = mpos.y / Tile::tileSize;

    // check for tile at coordinates
    if (floor.contains(coords.x) && floor[coords.x].contains(coords.y)
        && inRange(coords, playerCoords)) {
        activeTile = std::make_unique<sf::Vector2i>(coords);
    }
    else if (activeTile) {
        activeTile = nullptr;
    }

    return activeTile.get();
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

    makeWater();

    makeGrass();

    // add additional details here based on detail type (or tile type if there is no detail yet)

    std::cout << "\n\tfloor is made!";
}

void World::makeWater()
{
    size_t pondCount = 3;
    size_t i = 0;
    while (i++ < pondCount) {
        sf::Vector2i size(prng::number(8u, 24u), prng::number(8u, 24u));
        sf::Vector2i pos;
        pos.x = prng::number(worldMin.x + size.x, worldMax.x - (size.x * 2));
        pos.y = prng::number(worldMin.y + size.y, worldMax.y - (size.y * 2));

        size_t iterations = 9;
        float chance = .85f;

        sf::Vector2i padding(2, 2);

        Automaton pond_maker(iterations, chance, pos, pos + size, padding);

        std::cout << "making pond at " << pos << ", size " << size << '\n';

        Automaton_Cells pond = pond_maker.iterate();

        for (int x = pos.x - padding.x; x <= pos.x + size.x + (padding.x * 2); x++) {
            for (int y = pos.y - padding.y; y <= pos.y + size.y + (padding.y * 2); y++) {
                if (pond[x][y] && floor[x][y]) {
                    floor[x][y]->setType(Floor_Type::WATER);
                    floor[x][y]->setTexture(Texture_Manager::get("WATER"));
                    int r = roundFloat(Tile::tileSize);
                    sf::Vector2i r_size(r, r);
                    sf::Vector2i pos(0, 0);
                    pos.x = autotileX(pond[x][y - 1], pond[x - 1][y], pond[x][y + 1], pond[x + 1][y]);
                    floor[x][y]->setTextureRect(sf::IntRect(pos, r_size));
                }
            }
        }
    }
}

void World::makeGrass()
{
    size_t iterations = 5;
    float chance = .5f;
    sf::Vector2i padding(0, 0);
    Automaton grass_maker(iterations, chance, worldMin, worldMax, padding);
    Automaton_Cells grass = grass_maker.iterate();
    for (int x = worldMin.x; x <= worldMax.x; x++) {
        for (int y = worldMin.y; y <= worldMax.y; y++) {
            if (floor[x][y] && floor[x][y]->type == Floor_Type::DIRT && grass[x][y]) {
                floor[x][y]->details.push_back(Detail(Detail_Type::GRASS, Texture_Manager::get("GRASS")));
                floor[x][y]->details.back().setPosition(floor[x][y]->getPosition());
            }
        }
    }

    updateAutotiledDetails(worldMin, worldMax);
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

int World::autotileX(sf::Vector2i i, std::variant<Floor_Type, Detail_Type> type)
{
    /*
    // define a lambda for visiting the variant
    auto match = [&](auto&& x) -> bool { std::cout << static_cast<int>(x) << '\n'; return adjacentTileMatch(i, x); };
    i += sf::Vector2i(0, -1);
    bool n = std::visit(match, type);
    i += sf::Vector2i(-1, -1);
    bool w = std::visit(match, type);
    i += sf::Vector2i(1, 1);
    bool s = std::visit(match, type);
    i += sf::Vector2i(1, -1);
    bool e = std::visit(match, type);
    */

    bool n, w, s, e;

    if (std::holds_alternative<Floor_Type>(type)) {
        n = adjacentFloorMatch(i + sf::Vector2i(0, -1), std::get<Floor_Type>(type));
        w = adjacentFloorMatch(i + sf::Vector2i(-1, 0), std::get<Floor_Type>(type));
        s = adjacentFloorMatch(i + sf::Vector2i(0, 1), std::get<Floor_Type>(type));
        e = adjacentFloorMatch(i + sf::Vector2i(1, 0), std::get<Floor_Type>(type));
    }
    else if (std::holds_alternative<Detail_Type>(type)) {
        n = adjacentDetailMatch(i + sf::Vector2i(0, -1), std::get<Detail_Type>(type));
        w = adjacentDetailMatch(i + sf::Vector2i(-1, 0), std::get<Detail_Type>(type));
        s = adjacentDetailMatch(i + sf::Vector2i(0, 1), std::get<Detail_Type>(type));
        e = adjacentDetailMatch(i + sf::Vector2i(1, 0), std::get<Detail_Type>(type));
    }

    return autotileX(n, w, s, e);
}

int World::autotileX(bool n, bool w, bool s, bool e)
{
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
    return (floor[i.x][i.y] && floor[i.x][i.y]->details.size() > 0 && floor[i.x][i.y]->details.front().type == type);
}

bool World::adjacentFloorMatch(sf::Vector2i i, Floor_Type type)
{
    return (floor[i.x][i.y] && floor[i.x][i.y]->type == type);
}

void World::makeWalls()
{
    std::cout << "\nmaking walls...";
    std::cout << "\n\twalls made!";
}

Map_Tile<Floor>& World::getFloor()
{
    return floor;
}

Map_Tile<Wall>& World::getWalls()
{
    return walls;
}

Map_Tile<Crop>& World::getCrops()
{
    return crops;
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

std::vector<sf::FloatRect> World::getLocalImpassableTiles(sf::Vector2i p)
{
    std::vector<sf::FloatRect> tiles;

    const static int depth = 1;

    for (int x = p.x - depth; x <= p.x + depth; ++x) {
        for (int y = p.y - depth; y <= p.y + depth; ++y) {
            Tile* t = getWall(x, y);
            if (t != nullptr) {
                tiles.push_back(t->getGlobalBounds());
            }
            else {
                Floor* f = floor[x][y].get();
                if (f != nullptr && f->type == Floor_Type::WATER) {
                    tiles.push_back(f->getGlobalBounds());
                }
            }
        }
    }

    return tiles;
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
    if (activeTile) {
        sf::Vector2i t = *activeTile;
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
    if (activeTile) {
        sf::Vector2i t = *activeTile;
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
            int r = roundFloat(Tile::tileSize);
            sf::Vector2i pos(0, (item->getUID() % 100) * r);
            sf::Vector2i size(r, r);
            sprite.setTextureRect(sf::IntRect(pos, size));
            sprite.setOrigin(sf::Vector2f(size) / 2.f);
            sprite.setPosition(sf::Vector2f(t) * Tile::tileSize);

            item->take(1);
            f->planted = true;

            crops[t.x][t.y] = std::make_unique<Crop>(Crop(d));
            crops[t.x][t.y]->setSprite(sprite);
        }
    }
}

bool World::changeActiveTile(Floor_Type prereq, Floor_Type ntype)
{
    bool change = false;

    if (activeTile) {
        sf::Vector2i t = *activeTile;
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
            if (y.second) {
                sf::Vector2i c = y.second->getCoordinates();
                y.second->tick(floor[c.x][c.y]->type == Floor_Type::WATERED);
            }
        }
    }
}

void World::setInteracting(bool interacting)
{
    this->interacting = interacting;
}
