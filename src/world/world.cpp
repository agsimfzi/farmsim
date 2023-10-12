#include <world/world.hpp>

#include <cmath>
#include <iostream>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <util/primordial.hpp>
#include <util/prng.hpp>

#include <world/automaton.hpp>
#include <world/biome_generator.hpp>

#include <util/vector2_stream.hpp>

//////////////////////////////////////////////////////////////

World::World(Library& library)
    : library { library }
{
    sf::Vector2i size(16, 16);
    size.x *= chunks.chunk_size.x;
    size.y *= chunks.chunk_size.y;
    //size.x -= 1;
    size.y -= 1;
    world_min = -(size / 2);
    world_max = (size / 2);
    chunks.world_min = world_min;
    chunks.world_max = world_max;

    std::cout << "WORLD BOUNDS CALCULATED:\n\t" << world_min << " to " << world_max << "!\n";
}

void World::nextSeason(Player& player)
{
    chunks.clear();
    std::cout << "\n\nSEASON CHANGE, FROM " << seasonToString(season);
    int s = static_cast<int>(season);
    s++;
    if (s > 3) {
        s = 0;
        year++;
    }
    season = static_cast<Season>(s);
    std::cout << " TO " << seasonToString(season) << "!\n";
    killUnseasonableCrops();
    seasonalIteration();
    chunks.check(player.getCoordinates(tile_size));
    // reset all timers
}

void World::killUnseasonableCrops()
{
    for (auto& r : crops) {
        for (auto& c : r.second) {
            if (!c.second.checkSeason(season)) {
                c.second.kill();
            }
        }
    }
}

void World::seasonalIteration()
{
    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            Tile& info = tile_library[x][y];
            if (info.detail) {
                info.detail->setSeasonalTextureRect(season);
            }
        }
    }
}

void World::reset()
{
    tile_library.clear();
    chunks.clear();
}

void World::update(Player_Inventory& player_inventory, Player& player, float deltaTime)
{
    checkPickup(player_inventory, player, deltaTime);
    chunks.update();

    for (auto& v : vehicles) {
        v->update();
    }
}

sf::Vector2i World::worldMin()
{
    return world_min;
}

sf::Vector2i World::worldMax()
{
    return world_max;
}

sf::Vector2i* World::checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords)
{
    // future: pass player coordinates + tool distance
    //mpos += sf::Vector2f(sign(mpos.x) * tile_size, sign(mpos.y) * tile_size);

    for (int x = playerCoords.x - 1; x <= playerCoords.x + 1; x++) {
        for (int y = playerCoords.y - 1; y <= playerCoords.y + 1; y++) {
            if (tile_library[x][y].bounds.contains(mpos)) {
                active_tile = std::make_unique<sf::Vector2i>(x, y);
                return active_tile.get();
            }
        }
    }

    active_tile = nullptr;
    return active_tile.get();
}

bool World::inRange(sf::Vector2i c1, sf::Vector2i c2)
{
    sf::Vector2i diff = c1 - c2;

    return ((diff.x >= -1 && diff.x <= 1)
        && diff.y >= -1 && diff.y <= 1);
}

void World::makeBiomes()
{
    std::cout << "generating biomes\n";
    Biome_Generator biome_gen(world_min, world_max);
    Map_Tile<Biome>& biomes = biome_gen.generate();

    // reading tile from biome map
    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            sf::Vector2i c(x, y);
            Tile& info = tile_library[x][y];
            info.coordinates = c;
            info.planted = false;
            info.biome = biomes[x][y];

            if (info.biome == Biome::NULL_TYPE) {
                info.setFloor(Floor_Type::NULL_TYPE);
            }
            else if (info.biome == Biome::OCEAN || info.biome == Biome::LAKE || info.biome == Biome::RIVER) {
                info.setFloor(Floor_Type::WATER);
            }
            else if (info.biome == Biome::BEACH) {
                info.setFloor(Floor_Type::SAND);
                // palm trees
            }
            else if (info.biome == Biome::VOLCANO) {
                info.setFloor(Floor_Type::BASALT);

                if (prng::boolean(0.008f)) {
                    info.detail = std::make_unique<Detail>(library.detail("BASALT"));
                    info.detail->coordinates = c;
                }
                else if (prng::boolean(0.006f)) {
                    if (prng::boolean(.6f)) {
                        info.detail = std::make_unique<Detail>(library.detail("GOLD"));
                        info.detail->coordinates = c;
                    }
                    else {
                        info.detail = std::make_unique<Detail>(library.detail("IRON"));
                        info.detail->coordinates = c;
                    }
                }
            }
            else if (info.biome == Biome::CALDERA) {
                info.setFloor(Floor_Type::LAVA);
            }
            else {
                info.floor = Floor_Type::DIRT;

                if (!adjacentTree(c)) {
                    if (info.biome == Biome::GRASSLAND) {
                        if (prng::boolean(0.003f)) {
                            info.detail = std::make_unique<Detail>(library.detail("BIRCH"));
                            info.detail->coordinates = c;
                        }
                        else if (prng::boolean(0.003f)) {
                            info.detail = std::make_unique<Detail>(library.detail("LIMESTONE"));
                            info.detail->coordinates = c;
                        }
                        else if (prng::boolean(0.002f)) {
                            if (prng::boolean(.6f)) {
                                info.detail = std::make_unique<Detail>(library.detail("COPPER"));
                                info.detail->coordinates = c;
                            }
                            else {
                                info.detail = std::make_unique<Detail>(library.detail("IRON"));
                                info.detail->coordinates = c;
                            }
                        }
                    }
                    else if (info.biome == Biome::FOREST) {
                        if (prng::boolean(0.2f)) {
                            info.detail = std::make_unique<Detail>(library.detail("PINE"));
                            info.detail->coordinates = c;
                        }
                        else if (prng::boolean(0.002f)) {
                            info.detail = std::make_unique<Detail>(library.detail("GRANITE"));
                            info.detail->coordinates = c;
                        }
                        else if (prng::boolean(0.001f)) {
                            if (prng::boolean(.7f)) {
                                info.detail = std::make_unique<Detail>(library.detail("COPPER"));
                                info.detail->coordinates = c;
                            }
                            else {
                                info.detail = std::make_unique<Detail>(library.detail("IRON"));
                                info.detail->coordinates = c;
                            }
                        }
                    }
                }
            }
            info.texture_pos = sf::Vector2i(0, (static_cast<int>(info.floor)) * roundFloat(tile_size));

            info.bounds.left = c.x * tile_size;
            info.bounds.top = c.y * tile_size;
            info.bounds.width = tile_size;
            info.bounds.height = tile_size;
        }
    }
    autotile(world_min, world_max, Floor_Type::WATER);
    autotile(world_min, world_max, Floor_Type::LAVA);

    start_coords = biome_gen.getStartCoordinates();

    std::cout << "biomes made!\n";
}

void World::makeGrass()
{
    return;
    std::cout << "\n\nmaking grass!\n";
    size_t iterations = 0;
    float chance = 0.8f;
    sf::Vector2i padding(0, 0);
    Automaton grass_maker(iterations, chance, world_min, world_max, padding);
    grass_maker.make();
    Automaton_Cells grass = grass_maker.iterate();
    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            if (validLibraryTile(x, y)
            && tile_library[x][y].floor == Floor_Type::DIRT
            && grass[x][y]) {
                if (tile_library[x][y].biome == Biome::FOREST && prng::boolean(0.5f)) {
                    grass[x][y] = false;
                    continue;
                }
                tile_library[x][y].floor = Floor_Type::GRASS;
                tile_library[x][y].texture_pos.x = autotileX(sf::Vector2i(x, y), Floor_Type::GRASS);
                tile_library[x][y].texture_pos.y = static_cast<int>(Floor_Type::GRASS) * tile_size;
            }
        }
    }
    autotile(world_min, world_max, Floor_Type::GRASS);
    std::cout << "\n\ngrass made!\n";
}

void World::finalize()
{
    chunks.check(start_coords);
    placeWreckage();
}

sf::Vector2f World::startPosition()
{
    return sf::Vector2f(tile_library[start_coords.x][start_coords.y].coordinates) * tile_size;
}

void World::autotile(sf::Vector2i start, sf::Vector2i end, Floor_Type type)
{
    for (int x = start.x; x <= end.x; x++) {
        for (int y = start.y; y <= end.y; y++) {
            if (tile_library[x][y].floor == type) {
                Tile& info = tile_library[x][y];
                sf::Vector2i c(x, y);
                info.texture_pos.x = autotileX(c, type);
                chunks.updateTile(info);
            }
        }
    }
}

void World::placeWreckage()
{
    std::shared_ptr<Item> axe = library.item("axe");
    axe->can_pickup = false;

    int distance = 1;
    sf::Vector2i axe_coords = randomNearbyEmptyTile(start_coords, distance);
    chunks.addItem(axe, axe_coords);

    std::vector<std::shared_ptr<Item>> items;
    items.push_back(library.item("pickaxe"));
    items.back()->setCount(1);
    items.push_back(library.item("hammer"));
    items.back()->setCount(1);
    items.push_back(library.item("hoe"));
    items.back()->setCount(1);
    items.push_back(library.item("watering can"));
    items.back()->setCount(1);
    items.push_back(library.item(1000)); // carrot seeds
    items.back()->setCount(10);
    items.push_back(library.item(1001)); // melon seeds
    items.back()->setCount(10);
    items.push_back(library.item(1002)); // cotton seeds
    items.back()->setCount(10);

    sf::Vector2i coords;

    distance = 3;
    size_t n = items.size();
    for (size_t i = 0; i < n; i++) {
        do {
            coords = randomNearbyEmptyTile(start_coords, distance);
        } while (coords == axe_coords);
        std::shared_ptr<Lootable> lootable = std::dynamic_pointer_cast<Lootable>(library.building("crate"));
        lootable->addItem(items[i]);
        tile_library[coords.x][coords.y].building = lootable;
        chunks.addBuilding(tile_library[coords.x][coords.y].building, coords);
        distance += prng::number(0, 2);
    }
}

sf::Vector2i World::randomNearbyEmptyTile(sf::Vector2i i, int distance)
{
    std::vector<sf::Vector2i> empty;

    for (int x = i.x - distance; x <= i.x + distance; x++) {
        for (int y = i.y - distance; y <= i.y + distance; y++) {
            sf::Vector2i c(x, y);
            if (c != i && emptyTile(c)) {
                empty.push_back(c);
            }
        }
    }

    sf::Vector2i t = empty[prng::number(empty.size())];

    return t;
}

int World::autotileX(sf::Vector2i i, Floor_Type type)
{
    bool n = adjacentFloorMatch(i + sf::Vector2i(0, -1), type);
    bool w = adjacentFloorMatch(i + sf::Vector2i(-1, 0), type);
    bool s = adjacentFloorMatch(i + sf::Vector2i(0, 1), type);
    bool e = adjacentFloorMatch(i + sf::Vector2i(1, 0), type);

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

    return (sum * roundFloat(tile_size));
}

bool World::adjacentDetailMatch(sf::Vector2i i, Detail::Type type)
{
    return (validLibraryTile(i.x, i.y)
        && (tile_library[i.x][i.y].detail->getType() == type || tile_library[i.x][i.y].biome == Biome::NULL_TYPE));
}

bool World::adjacentBiomeMatch(sf::Vector2i i, Biome type)
{
    return (validLibraryTile(i.x, i.y) && tile_library[i.x][i.y].biome == type);
}

bool World::adjacentFloorMatch(sf::Vector2i i, Floor_Type type)
{
    return (validLibraryTile(i.x, i.y) && tile_library[i.x][i.y].floor == type);
}

bool World::validLibraryTile(int x, int y)
{
    return (tile_library.contains(x) && tile_library[x].contains(y));
}

Map_Tile<Tile>& World::getTileLibrary()
{
    return tile_library;
}

Map_Tile<Crop>& World::getCrops()
{
    return crops;
}

std::vector<std::shared_ptr<Machine>>& World::getMachines()
{
    return machines;
}

std::vector<std::shared_ptr<Vehicle>>& World::getVehicles()
{
    return vehicles;
}

std::vector<sf::FloatRect> World::getLocalImpassableTiles(sf::Vector2i p)
{
    std::vector<sf::FloatRect> local_tiles;

    const static int depth = 2;

    for (int x = p.x - depth; x <= p.x + depth; ++x) {
        for (int y = p.y - depth; y <= p.y + depth; ++y) {
            sf::Vector2i c(x, y);
            if (!passableTile(c)) {
                local_tiles.push_back(tile_library[c.x][c.y].bounds);
            }
        }
    }

    return local_tiles;
}

std::vector<std::pair<Tile, sf::FloatRect>> World::getLocalTiles(sf::Vector2i p)
{
    std::vector<std::pair<Tile, sf::FloatRect>> local_tiles;

    const static int depth = 2;

    for (int x = p.x - depth; x <= p.x + depth; ++x) {
        for (int y = p.y - depth; y <= p.y + depth; ++y) {
            sf::Vector2i c(x, y);
            local_tiles.push_back(std::make_pair(tile_library[c.x][c.y], tile_library[c.x][c.y].bounds));
        }
    }

    return local_tiles;
}

void World::addCrop(sf::Vector2i c, Crop crop)
{
    crops[active_tile->x][active_tile->y] = crop;
    crops[active_tile->x][active_tile->y].place(c, sf::Vector2f(c) * tile_size);
}

bool World::changeActiveTile(Floor_Type prereq, Floor_Type ntype)
{
    bool change = false;

    if (active_tile) {
        Tile& info = tile_library[active_tile->x][active_tile->y];
        change = info.floor == prereq;
        if (change) {
            if (info.planted && ntype != Floor_Type::WATERED) {
                info.planted = false;
                removeCrop(*active_tile);
            }
            info.setFloor(ntype);
            chunks.updateTile(info);
        }
    }

    return change;
}

void World::removeCrop(sf::Vector2i i)
{
    tile_library[i.x][i.y].planted = false;
    crops[i.x].erase(i.y);
    if (crops[i.x].size() == 0) {
        crops.erase(i.x);
    }
}

Season World::getSeason()
{
    return season;
}

void World::tick(sf::Vector2i player_coordinates)
{
    for (auto& x : crops) {
        for (auto& y : x.second) {
            sf::Vector2i c = y.second.getCoordinates();
            y.second.tick(tile_library[c.x][c.y].floor == Floor_Type::WATERED);
        }
    }

    for (auto m = machines.begin(); m != machines.end();) {
        if (!(*m)) {
            machines.erase(m);
        }
        else {
            (*m)->tick(library.items);
            m++;
        }
    }

    chunks.check(player_coordinates);
}

void World::checkPickup(Player_Inventory& inventory, Player& player, float deltaTime)
{
    chunks.checkPickup(inventory, player, pickup_all, deltaTime);
}

Chunk_Loader& World::getChunks()
{
    return chunks;
}

void World::pickupAll()
{
    pickup_all = true;
}

void World::stopPickupAll()
{
    pickup_all = false;
}

sf::Vector2i* World::activeTile()
{
    return active_tile.get();
}

void World::setInteractable()
{
    if (active_tile) {
        setActiveBuilding();
        if (!active_building) {
            setActiveNPC();
        }
    }
}

void World::setActiveBuilding()
{
    if (active_tile) {
        sf::Vector2i t = *active_tile;
        Building* b = tile_library[t.x][t.y].building.get();
        if (b) {
            active_building = b;
        }
        else {
            active_building = nullptr;
        }
    }
}

void World::setActiveNPC()
{
    const auto& t = *active_tile;
    for (const auto& npc : npcs) {
        if (t == npc->getCoordinates(tile_size)) {
            active_npc = npc.get();
            return;
        }
    }
    active_npc = nullptr;
}

void World::closeActiveBuilding()
{
    active_building = nullptr;
}

Building* World::activeBuilding()
{
    return active_building;
}

NPC* World::activeNPC()
{
    return active_npc;
}

bool World::adjacentTree(sf::Vector2i i)
{
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            Detail* d = tile_library[i.x + x][i.y + y].detail.get();
            if (d && d->getType() == Detail::TREE) {
                return true;
            }
        }
    }
    return false;
}

bool World::emptyTile(sf::Vector2i i)
{
    return emptyTile(tile_library[i.x][i.y]);
}

bool World::emptyTile(Tile& info)
{
    return (!info.detail
         && !info.building
         && info.floor != Floor_Type::WATER
         && info.floor != Floor_Type::LAVA
         && info.biome != Biome::NULL_TYPE);
}

bool World::buildableTile(sf::Vector2i i)
{
    return buildableTile(tile_library[i.x][i.y]);
}

bool World::buildableTile(Tile& info)
{
    return (emptyTile(info) && !plantableTile(info));
}

bool World::plantableTile(sf::Vector2i i)
{
    return plantableTile(tile_library[i.x][i.y]);
}

bool World::plantableTile(Tile& info)
{
    return (!info.planted && (info.floor == Floor_Type::TILLED || info.floor == Floor_Type::WATERED));
}

bool World::passableTile(sf::Vector2i i)
{
    return passableTile(tile_library[i.x][i.y]);
}

bool World::passableTile(Tile& info)
{
    bool passable = emptyTile(info);
    if (info.planted && !crops[info.coordinates.x][info.coordinates.y].passable())
    {
        passable = false;
    }
    return passable;
}
