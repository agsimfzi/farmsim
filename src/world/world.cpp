#include <world/world.hpp>

#include <cmath>
#include <iostream>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <util/primordial.hpp>
#include <util/prng.hpp>
#include <util/vector2_stream.hpp>

#include <world/automaton.hpp>
#include <world/biome_generator.hpp>

//////////////////////////////////////////////////////////////

World::World(Item_Library& item_library)
    : item_library { item_library }
{
    sf::Vector2i size(16, 16);
    size.x *= chunks.chunk_size.x;
    size.y *= chunks.chunk_size.y;
    size.y -= 1;
    world_min = -(size / 2);
    world_max = (size / 2);
    chunks.world_min = world_min;
    chunks.world_max = world_max;

    std::vector<Rock_Data> rd = Database::getRockData();
    for (auto& d : rd) {
        Rock::Type t = Rock::stringToType(d.name);
        rock_data[t] = d;
    }

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
    }
    season = static_cast<Season>(s);
    std::cout << " TO " << seasonToString(season) << "!\n";
    killUnseasonableCrops();
    seasonalIteration();
    chunks.check(player.getCoordinates(Tile::tile_size), season);
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
            Floor_Info& info = tile_library[x][y];
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

    /*
    for (auto& m : machines) {
        //m->update();
    }
    */

    for (auto& v : vehicles) {
        v->update();
    }
}

void World::interact(Player& player, Player_Inventory& player_inventory)
{
    if (active_tile) {
        sf::Vector2i t = *active_tile;
        Floor* f = chunks.floor(*active_tile);
        Floor_Info& info = tile_library[t.x][t.y];
        std::shared_ptr<Vehicle> pv = player.getVehicle();
        if (pv) { // CHECK FOR VEHICLE DISMOUNT
            if (emptyTile(info)
            && (emptyTile(player.getCoordinates(Tile::tile_size)) || pv->type == Vehicle::BOAT)) {
                player.setVehicle(nullptr);
                player.setPosition(f->getPosition());
                vehicles.push_back(pv);
                pv = nullptr;
            }
        }
        else { // CHECK FOR VEHICLE MOUNT
            for (auto v = vehicles.begin(); v != vehicles.end();) {
                if ((*v) && f->getGlobalBounds().contains((*v)->getPosition())) {
                    player.setVehicle((*v));
                    vehicles.erase(v);
                    return;
                }
                else {
                    v++;
                }
            }

            // CHECK FOR MACHINE INTERACTION

            // INHERIT "INTERACTABLE" FROM BOTH OBJECTS
            // IMPLEMENT AN OPTIONAL CALLBACK
        }
        if (f) {
            std::shared_ptr<Item> i = player_inventory.equippedItem();
            if (info.building && info.building->type == Building::MACHINE) { // PICKUP PRODUCTS
                auto m = std::dynamic_pointer_cast<Machine>(info.building);
                std::shared_ptr<Item> p = m->activeProduct();
                if (p) {
                    player_inventory.addItem(p);
                    m->setProduct(p);
                }
            } // END PICKUP PRODUCTS
            else if (f->planted) { // HARVEST
                if (!crops[t.x].contains(t.y)) {
                    std::cout << "FAILED TO FIND CROP AT TILE " << t << '\n';
                }
                else {
                    Crop& crop = crops[t.x][t.y];
                    if (crop.fullyGrown()) {
                        std::shared_ptr<Item> i = std::make_shared<Item>(*item_library.item(crops[t.x][t.y].harvestUID()));
                        i->setCount(crop.getQuantity());
                        player_inventory.addItem(i);
                        if (i) {
                            chunks.addItem(i, player.getCoordinates(Tile::tile_size));
                        }
                        if (crop.regrows()) {
                            crop.harvestRegrowable();
                        }
                        else {
                            removeCrop(t);
                        }
                    }
                }
            } // END HARVEST
        }
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
    mpos += sf::Vector2f(sign(mpos.x) * (Tile::tile_size / 2.f), sign(mpos.y) * (Tile::tile_size / 2.f));
    sf::Vector2i coords;
    coords.x = mpos.x / Tile::tile_size;
    coords.y = mpos.y / Tile::tile_size;

    // check for tile at coordinates
    if (inRange(coords, playerCoords) && chunks.floor(coords)) {
        active_tile = std::make_unique<sf::Vector2i>(coords);
    }
    else if (active_tile) {
        active_tile = nullptr;
    }

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

    // reading tile_info from biome map
    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            sf::Vector2i coords(x, y);
            Floor_Info& info = tile_library[x][y];
            info.coordinates = coords;
            info.planted = false;
            info.biome = biomes[x][y];
            info.detail_pos = sf::Vector2i(0, 0);

            Rock::Type rock = Rock::NULL_TYPE;
            Tree::Type tree = Tree::NULL_TYPE;

            if (info.biome == Biome::NULL_TYPE) {
                info.floor = Floor_Type::NULL_TYPE;
            }
            else if (info.biome == Biome::OCEAN || info.biome == Biome::LAKE || info.biome == Biome::RIVER) {
                info.floor = Floor_Type::WATER;
            }
            else if (info.biome == Biome::BEACH) {
                info.floor = Floor_Type::SAND;
                // palm trees
            }
            else if (info.biome == Biome::VOLCANO) {
                info.floor = Floor_Type::BASALT;

                if (prng::boolean(0.008f)) {
                    rock = Rock::BASALT;
                }
                else if (prng::boolean(0.006f)) {
                    rock = Rock::GOLD;
                }
            }
            else if (info.biome == Biome::CALDERA) {
                info.floor = Floor_Type::LAVA;
            }
            else {
                info.floor = Floor_Type::DIRT;

                if (info.biome == Biome::GRASSLAND) {
                    if (prng::boolean(0.003f)) {
                        tree = Tree::BIRCH;
                    }
                    else if (prng::boolean(0.003f)) {
                        rock = Rock::LIMESTONE;
                    }
                    else if (prng::boolean(0.002f)) {
                        if (prng::boolean(.6f)) {
                            rock = Rock::COPPER;
                        }
                        else {
                            rock = Rock::IRON;
                        }
                    }
                }
                else if (info.biome == Biome::FOREST) {
                    if (prng::boolean(0.2f)) {
                        tree = Tree::PINE;
                    }
                    else if (prng::boolean(0.002f)) {
                        rock = Rock::GRANITE;
                    }
                    else if (prng::boolean(0.001f)) {
                        if (prng::boolean(.7f)) {
                            rock = Rock::COPPER;
                        }
                        else {
                            rock = Rock::IRON;
                        }
                    }
                }
            }
            info.texture_pos = sf::Vector2i(0, (static_cast<int>(info.floor)) * roundFloat(Tile::tile_size));
            info.detail_pos = sf::Vector2i(0, 0);

            info.tree = tree;
            if (rock != Rock::NULL_TYPE) {
                info.rock = std::make_shared<Rock>(coords, Texture_Manager::get("ROCKS"), rock, rock_data[rock]);
            }
        }
    }
    autotile(world_min, world_max, Floor_Type::WATER);
    autotile(world_min, world_max, Floor_Type::LAVA);

    start_coords = biome_gen.getStartCoordinates();

    std::cout << "biomes made!\n";
}

void World::makeGrass()
{
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
            && !(tile_library[x][y].tree != Tree::Type::NULL_TYPE)
            && grass[x][y]) {
                if (tile_library[x][y].biome == Biome::FOREST && prng::boolean(0.5f)) {
                    grass[x][y] = false;
                    continue;
                }
                tile_library[x][y].floor = Floor_Type::GRASS;
                tile_library[x][y].texture_pos.x = autotileX(sf::Vector2i(x, y), Floor_Type::GRASS);
                tile_library[x][y].texture_pos.y = static_cast<int>(Floor_Type::GRASS) * Tile::tile_size;
            }
        }
    }
    autotile(world_min, world_max, Floor_Type::GRASS);
    std::cout << "\n\ngrass made!\n";
}

void World::finalize()
{
    chunks.check(start_coords, season);
    placeWreckage();
}

sf::Vector2f World::startPosition()
{
    return chunks.floor(start_coords)->getPosition();
}

void World::autotile(sf::Vector2i start, sf::Vector2i end, Floor_Type type)
{
    for (int x = start.x; x <= end.x; x++) {
        for (int y = start.y; y <= end.y; y++) {
            if (tile_library[x][y].floor == type) {
                Floor_Info& info = tile_library[x][y];
                sf::Vector2i c(x, y);
                info.texture_pos.x = autotileX(c, type);
            }
        }
    }
}

void World::placeWreckage()
{
    std::shared_ptr<Item> axe = std::make_shared<Item>(*item_library("axe"));
    axe->can_pickup = false;

    int distance = 1;
    sf::Vector2i axe_coords = randomNearbyEmptyTile(start_coords, distance);
    chunks.addItem(axe, axe_coords);

    std::vector<std::shared_ptr<Item>> items;
    items.push_back(item_library.shared("pickaxe"));
    items.back()->setCount(1);
    items.push_back(item_library.shared("hammer"));
    items.back()->setCount(1);
    items.push_back(item_library.shared("hoe"));
    items.back()->setCount(1);
    items.push_back(item_library.shared("watering can"));
    items.back()->setCount(1);
    items.push_back(item_library.shared(1000)); // carrot seeds
    items.back()->setCount(10);
    items.push_back(item_library.shared(1001)); // melon seeds
    items.back()->setCount(10);
    items.push_back(item_library.shared(1002)); // cotton seeds
    items.back()->setCount(10);

    sf::Vector2i coords;

    distance = 3;
    size_t n = items.size();
    for (size_t i = 0; i < n; i++) {
        do {
            coords = randomNearbyEmptyTile(start_coords, distance);
        } while (coords == axe_coords);
        std::shared_ptr<Lootable> lootable = std::dynamic_pointer_cast<Lootable>((*building_library)("crate"));
        lootable->addItem(items[i]);
        tile_library[coords.x][coords.y].building = lootable;
        chunks.addBuilding(tile_library[coords.x][coords.y].building, coords);
        distance += prng::number(0, 2);
    }
}

Building_Library& World::getBuildingLibrary()
{
    return *building_library;
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

    return (sum * roundFloat(Tile::tile_size));
}

bool World::adjacentDetailMatch(sf::Vector2i i, Detail_Type type)
{
    return (validLibraryTile(i.x, i.y)
        && (tile_library[i.x][i.y].detail == type || tile_library[i.x][i.y].biome == Biome::NULL_TYPE));
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

Map_Tile<Floor_Info>& World::getTileLibrary()
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

    const static int depth = 1;

    for (int x = p.x - depth; x <= p.x + depth; ++x) {
        for (int y = p.y - depth; y <= p.y + depth; ++y) {
            sf::Vector2i c(x, y);
            Floor* f = chunks.floor(c);
            if (f && !passableTile(c)) {
                local_tiles.push_back(f->getGlobalBounds());
            }
        }
    }

    return local_tiles;
}

std::vector<std::pair<Floor_Info, sf::FloatRect>> World::getLocalTiles(sf::Vector2i p)
{
    std::vector<std::pair<Floor_Info, sf::FloatRect>> local_tiles;

    const static int depth = 1;

    for (int x = p.x - depth; x <= p.x + depth; ++x) {
        for (int y = p.y - depth; y <= p.y + depth; ++y) {
            sf::Vector2i c(x, y);
            Floor* f = chunks.floor(c);
            if (f) {
                local_tiles.push_back(std::make_pair(tile_library[x][y], f->getGlobalBounds()));
            }
        }
    }

    return local_tiles;
}

void World::addCrop(sf::Vector2i c, Crop crop)
{
    crops[active_tile->x][active_tile->y] = crop;
    crops[active_tile->x][active_tile->y].place(c, chunks.floor(c)->getPosition());
}

bool World::changeActiveTile(Floor_Type prereq, Floor_Type ntype)
{
    bool change = false;

    if (active_tile) {
        Floor* f = chunks.floor(*active_tile);
        change = (f->type == prereq);
        if (change) {
            if (f->planted && ntype != Floor_Type::WATERED) {
                f->planted = false;
                removeCrop(*active_tile);
            }
            f->setType(ntype);
            tileToLibrary(f);
        }
    }

    return change;
}

void World::setBuildingLibrary(Building_Library* b)
{
    building_library = b;
}

void World::removeCrop(sf::Vector2i i)
{
    tile_library[i.x][i.y].planted = false;
    chunks.floor(i)->planted = false;
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
            y.second.tick(chunks.floor(c)->type == Floor_Type::WATERED);
        }
    }

    for (auto m = machines.begin(); m != machines.end();) {
        if (!(*m)) {
            machines.erase(m);
        }
        else {
            (*m)->tick(item_library);
            m++;
        }
    }

    chunks.check(player_coordinates, season);
}

void World::setInteracting(bool interacting)
{
    this->interacting = interacting;
}

Floor* World::activeFloor(sf::Vector2i i)
{
    Floor* f = nullptr;

    if (active_tile) {
        f = chunks.floor(*active_tile);
    }

    return f;
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

void World::closeActiveBuilding()
{
    active_building = nullptr;
}

Building* World::activeBuilding()
{
    return active_building;
}

void World::tileToLibrary(sf::Vector2i i)
{
    tileToLibrary(chunks.floor(i));
}

void World::tileToLibrary(Floor* f)
{
    if (f) {
        sf::Vector2i c = f->coordinates;
        Floor_Info& info = tile_library[c.x][c.y];
        info.planted = f->planted;
        info.floor = f->type;
        info.detail = f->detail;
        info.texture_pos.x = f->getTextureRect().left;
        info.texture_pos.y = f->getTextureRect().top;

        if (info.detail != Detail_Type::NULL_TYPE) {
            Detail* d = chunks.detail(c);
            if (d) {
                info.detail_pos.x = d->getTextureRect().left;
                info.detail_pos.y = d->getTextureRect().top;
            }
        }
    }
}

bool World::adjacentTree(sf::Vector2i i)
{
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (tile_library[i.x + x][i.y + y].tree != Tree::Type::NULL_TYPE) {
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

bool World::emptyTile(Floor_Info& info)
{
    return (!(info.tree != Tree::Type::NULL_TYPE)
         && !info.rock
         && !info.building
         && info.floor != Floor_Type::WATER
         && info.floor != Floor_Type::LAVA
         && info.biome != Biome::NULL_TYPE);
}

bool World::buildableTile(sf::Vector2i i)
{
    return buildableTile(tile_library[i.x][i.y]);
}

bool World::buildableTile(Floor_Info& info)
{
    return (emptyTile(info) && !plantableTile(info));
}

bool World::plantableTile(sf::Vector2i i)
{
    return plantableTile(tile_library[i.x][i.y]);
}

bool World::plantableTile(Floor_Info& info)
{
    return (!info.planted && (info.floor == Floor_Type::TILLED || info.floor == Floor_Type::WATERED));
}

bool World::passableTile(sf::Vector2i i)
{
    return passableTile(tile_library[i.x][i.y]);
}

bool World::passableTile(Floor_Info& info)
{
    bool passable = emptyTile(info);
    if (info.planted && !crops[info.coordinates.x][info.coordinates.y].passable())
    {
        passable = false;
    }
    return passable;
}
