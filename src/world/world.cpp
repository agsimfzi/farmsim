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
    sf::Vector2i size(32, 32);
    size.x *= chunks.chunk_size.x;
    size.y *= chunks.chunk_size.y;
    size.y -= 1;
    world_min = -(size / 2);
    world_max = (size / 2);
    chunks.world_min = world_min;
    chunks.world_max = world_max;

    std::cout << "WORLD BOUNDS CALCULATED:\n\t" << world_min << " to " << world_max << "!\n";
}

void World::reset()
{
    tile_library.clear();
    chunks.clear();
}

void World::update(Player_Inventory& player_inventory, Player& player)
{
    checkPickup(player_inventory, player);

    player.energy -= energyDiff();
    if (player.energy < 0) {
        player.energy = 0;
    }
    energy = player.energy;
}

void World::interact(Player& player, Player_Inventory& player_inventory, std::shared_ptr<Vehicle>& active_vehicle)
{
    if (activeTile) {
        sf::Vector2i t = *activeTile;
        Floor* f = chunks.floor(*activeTile);
        Floor_Info& info = tile_library[t.x][t.y];
        if (active_vehicle) { // CHECK FOR VEHICLE DISMOUNT
            if (emptyTile(info) && emptyTile(player.getCoordinates(Tile::tileSize))) {
                player.setVehicle(nullptr);
                player.setPosition(f->getPosition());
                vehicles.push_back(active_vehicle);
                active_vehicle = nullptr;
            }
        }
        else { // CHECK FOR VEHICLE MOUNT
            for (auto v = vehicles.begin(); v != vehicles.end();) {
                if ((*v) && f->getGlobalBounds().contains((*v)->getPosition())) {
                    player.setVehicle((*v).get());
                    active_vehicle = (*v);
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
                    player_inventory.addItem(p, p->count());
                    m->clearProduct();
                }
            } // END PICKUP PRODUCTS
            else if (f->planted) { // HARVEST
                if (!crops[t.x].contains(t.y)) {
                    std::cout << "FAILED TO FIND CROP AT TILE " << t << '\n';
                }
                else if (crops[t.x][t.y].fullyGrown()) {
                    player_inventory.addItem(std::make_shared<Item>(*item_library.item(crops[t.x][t.y].harvestUID())));
                    crops[t.x].erase(t.y);
                    f->setType(Floor_Type::TILLED);
                    f->planted = false;
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
    mpos += sf::Vector2f(sign(mpos.x) * (Tile::tileSize / 2.f), sign(mpos.y) * (Tile::tileSize / 2.f));
    sf::Vector2i coords;
    coords.x = mpos.x / Tile::tileSize;
    coords.y = mpos.y / Tile::tileSize;

    // check for tile at coordinates
    if (inRange(coords, playerCoords) && chunks.floor(coords)) {
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

            if (info.biome == Biome::NULL_TYPE) {
                info.floor = Floor_Type::NULL_TYPE;
            }
            else if (info.biome == Biome::OCEAN || info.biome == Biome::LAKE || info.biome == Biome::RIVER) {
                info.floor = Floor_Type::SAND;
                info.detail = Detail_Type::WATER;
            }
            else if (info.biome == Biome::BEACH) {
                info.floor = Floor_Type::SAND;
            }
            else if (info.biome == Biome::VOLCANO) {
                info.floor = Floor_Type::BASALT;
            }
            else if (info.biome == Biome::CALDERA) {
                info.floor = Floor_Type::BASALT;
                info.detail = Detail_Type::LAVA;
            }
            else {
                info.floor = Floor_Type::DIRT;

                if (prng::boolean(0.003f)) {
                    info.rock = true;
                }
                else if (((info.biome == Biome::FOREST && prng::boolean(0.2f))
                || (info.biome == Biome::GRASSLAND && prng::boolean(0.003f)))
                && !adjacentTree(coords)) {
                    info.tree = true;
                }
            }
            info.texture_pos = sf::Vector2i(0, (static_cast<int>(info.floor)) * roundFloat(Tile::tileSize));
            info.detail_pos = sf::Vector2i(0, 0);
        }
    }
    autotile(world_min, world_max, Detail_Type::WATER);
    autotile(world_min, world_max, Detail_Type::LAVA);

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
            if (validLibraryTile(x, y) && tile_library[x][y].floor == Floor_Type::DIRT && !tile_library[x][y].tree && grass[x][y]) {
                if (tile_library[x][y].biome == Biome::FOREST && prng::boolean(0.5f)) {
                        grass[x][y] = false;
                    continue;
                }
                tile_library[x][y].detail = Detail_Type::GRASS;
                tile_library[x][y].detail_pos.x = autotileX(sf::Vector2i(x, y), Detail_Type::GRASS);
            }
        }
    }
    autotile(world_min, world_max, Detail_Type::GRASS);
    std::cout << "\n\ngrass made!\n";
}

void World::finalize()
{
    chunks.check(start_coords);
    placeWreckage();
}

sf::Vector2f World::startPosition()
{
    return chunks.floor(start_coords)->getPosition();
}

void World::autotile(sf::Vector2i start, sf::Vector2i end, Detail_Type type)
{
    for (int x = start.x; x <= end.x; x++) {
        for (int y = start.y; y <= end.y; y++) {
            sf::Vector2i c(x, y);
            Detail* d = chunks.detail(c);
            if (d && d->type == type) {
                sf::Vector2i pos(autotileX(c, d->type), 0);
                sf::Vector2i size(Tile::tileSize, Tile::tileSize);
                d->setTextureRect(sf::IntRect(pos, size));
            }
            if (tile_library[x][y].detail == type) {
                Floor_Info& info = tile_library[x][y];
                info.detail_pos.x = autotileX(c, type);
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
    chunks.addItem(axe, 1, axe_coords);

    std::vector<std::shared_ptr<Item>> items;
    items.push_back(item_library.shared("pickaxe"));
    items.back()->setCount(1);
    items.push_back(item_library.shared("hammer"));
    items.back()->setCount(1);
    items.push_back(item_library.shared("hoe"));
    items.back()->setCount(1);
    items.push_back(item_library.shared("watering can"));
    items.back()->setCount(1);
    items.push_back(item_library.shared(1000));
    items.back()->setCount(10);
    items.push_back(item_library.shared(1001));
    items.back()->setCount(10);
    //items.push_back(item_library.shared("chest"));
    //items.push_back(item_library.shared("workbench"));
    //items.push_back(item_library.shared("furnace"));
    //items.push_back(item_library.shared("table saw"));
    //items.push_back(item_library.shared("anvil"));

    sf::Vector2i coords;

    distance = 3;
    size_t n = items.size();
    for (size_t i = 0; i < n; i++) {
        do {
            coords = randomNearbyEmptyTile(start_coords, distance);
        } while (coords == axe_coords);
        std::shared_ptr<Lootable> lootable = std::make_shared<Lootable>(*std::dynamic_pointer_cast<Lootable>(building_library("crate")));
        lootable->getInventory().front().front() = items[i];
        lootable->getInventory().front().push_back(item_library.shared("plank"));
        lootable->getInventory().front().back()->setCount(prng::number(1, 3));
        tile_library[coords.x][coords.y].building = lootable;
        chunks.addBuilding(tile_library[coords.x][coords.y].building.get(), coords);
        lootable.reset();
        distance += prng::number(0, 2);
    }
}

Building_Library& World::getBuildingLibrary()
{
    return building_library;
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

int World::autotileX(sf::Vector2i i, Detail_Type type)
{
    bool n = adjacentDetailMatch(i + sf::Vector2i(0, -1), type);
    bool w = adjacentDetailMatch(i + sf::Vector2i(-1, 0), type);
    bool s = adjacentDetailMatch(i + sf::Vector2i(0, 1), type);
    bool e = adjacentDetailMatch(i + sf::Vector2i(1, 0), type);

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
    return (validLibraryTile(i.x, i.y)
        && (tile_library[i.x][i.y].detail == type || tile_library[i.x][i.y].biome == Biome::NULL_TYPE));
}

bool World::adjacentBiomeMatch(sf::Vector2i i, Biome type)
{
    return (validLibraryTile(i.x, i.y) && tile_library[i.x][i.y].biome == type);
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

bool World::useItem(std::shared_ptr<Item> item)
{
    bool used = true;
    if (activeTile) {
        switch (item->getType()) {
            case Item_Type::TOOL:
                useTool(item);
                break;
            case Item_Type::SEED:
                plantCrop(item);
                break;
            case Item_Type::VEHICLE:
                useVehicle(item);
                break;
            case Item_Type::BUILDING:
                useBuilding(item);
                break;
            case Item_Type::RAW_MATERIAL:
                useRawMaterial(item);
                break;

            default:
                used = false;
                break;
        }
    }
    return used;
}

void World::useRawMaterial(std::shared_ptr<Item> item)
{
    Floor_Info& info = tile_library[activeTile->x][activeTile->y];
    if (info.building && info.building->type == Building::MACHINE) {
        auto m = std::dynamic_pointer_cast<Machine>(info.building);
        m->addReagant(item);
    }
}

void World::useBuilding(std::shared_ptr<Item> item)
{
    sf::Vector2i t = *activeTile;
    Floor_Info& info = tile_library[t.x][t.y];
    if (buildableTile(info)) { // PLACE BUILDING
        info.building = building_library(item->getUID());
        chunks.addBuilding(info.building.get(), t);
        item->take(1);
        if (info.building->type == Building::MACHINE) {
            machines.push_back(std::dynamic_pointer_cast<Machine>(info.building));
        }
    }
}

void World::useVehicle(std::shared_ptr<Item> item)
{
    switch (item->getUID()) {
        default:
            break;
        case 10000: // boat
            if (tile_library[activeTile->x][activeTile->y].detail == Detail_Type::WATER) {
                vehicles.push_back(std::make_shared<Vehicle>(Vehicle::BOAT, chunks.floor(*activeTile)->getPosition()));
                item->take(1);
            }
            break;
        case 10001: // boat
            if (emptyTile(*activeTile)) {
                vehicles.push_back(std::make_shared<Vehicle>(Vehicle::BROOM, chunks.floor(*activeTile)->getPosition()));
                item->take(1);
            }
            break;
    }
}

void World::useTool(std::shared_ptr<Item> item)
{
    if (energy > 0) {
        energy_diff = item->useFactor();
        switch (item->getUID()) {
            default:
                break;
            case 0: // hoe
                hoe();
                break;
            case 1: // watering can
                water(item);
                break;
            case 2: // axe
                axe(item->useFactor());
                break;
            case 3: // pick
                pick(item->useFactor());
                break;
            case 4: // hammer
                hammer();
                break;
        }
    }
}

int World::energyDiff()
{
    int d = energy_diff;
    energy_diff = 0;
    return d;
}

void World::hoe()
{
    sf::Vector2i t = *activeTile;
    if (tile_library[t.x][t.y].floor == Floor_Type::DIRT && emptyTile(t)) {
        Floor* f = chunks.floor(*activeTile);
        if (f->detail == Detail_Type::GRASS) {
            f->detail = Detail_Type::NULL_TYPE;
            chunks.eraseDetail(f->coordinates);
            tile_library[t.x][t.y].detail = Detail_Type::NULL_TYPE;
            autotile(t - sf::Vector2i(1, 1), t + sf::Vector2i(1, 1), Detail_Type::GRASS);
        }
        else {
            f->setType(Floor_Type::TILLED);
            tileToLibrary(t);
        }
    }
}

void World::water(std::shared_ptr<Item> item)
{
    if (tile_library[activeTile->x][activeTile->y].detail == Detail_Type::WATER) {
        item->resetUses();
    }
    else if (item->usePercent() > 0) {
        item->reduceUses();
        changeActiveTile(Floor_Type::TILLED, Floor_Type::WATERED);
    }
}

void World::axe(int factor)
{
    sf::Vector2i t = *activeTile;
    Floor_Info& info = tile_library[t.x][t.y];
    if (info.tree) {
        Tree* tree = chunks.tree(t);
        tree->hit(factor);
        if (tree->dead()) {
            // create logs on ground
            // create stump
            info.tree = false;
            chunks.eraseTree(t);
            Item* i = item_library.item("wood");
            std::shared_ptr<Item> item = std::make_shared<Item>(*i);
            size_t count = prng::number(7, 13);
            chunks.addItem(item, count, t);
        }
    }
    else if (info.building) {
        std::shared_ptr<Building> b = info.building;
        if (b->type == Building::LOOTABLE) {
            b->health -= factor;
            if (b->health <= 0) {
                for (auto& i : b->getInventory().front()) {
                    chunks.addItem(i, i->count(), t);
                }
                info.building.reset();
                chunks.eraseBuilding(t);
            }
        }
    }
}

void World::pick(int factor)
{
    if (!changeActiveTile(Floor_Type::TILLED, Floor_Type::DIRT)) {
        sf::Vector2i t = *activeTile;
        if (tile_library[t.x][t.y].rock) {
            Rock* rock = chunks.rock(t);
            if (rock) {
                rock->hit(factor);
                if (rock->dead()) {
                    tile_library[t.x][t.y].rock = false;
                    chunks.eraseRock(t);
                    std::string key = "stone";
                    size_t count = prng::number(2, 4);
                    unsigned int ore_roll = prng::number(0u, 100u);
                    if (ore_roll < 15) {
                        key = "copper ore";
                        count = prng::number(3, 4);
                    }
                    else if (ore_roll < 27) {
                        key = "iron ore";
                        count = prng::number(2, 3);
                    }
                    else if (ore_roll < 36) {
                        key = "gold ore";
                        count = prng::number(1, 2);
                    }
                    std::shared_ptr<Item> item = std::make_shared<Item>(*item_library.item(key));
                    chunks.addItem(item, count, t);

                    if (prng::boolean()) { // INDEPENDENT COAL-SPAWN CHANCE
                        std::shared_ptr<Item> item = std::make_shared<Item>(*item_library.item("coal"));
                        chunks.addItem(item, prng::number(3, 7), t);
                    }
                }
            }
        }
    }
}

void World::hammer()
{
    if (activeTile) {
        sf::Vector2i t = *activeTile;
        Building* b = tile_library[t.x][t.y].building.get();
        if (b) {
            if (b->type == Building::CONTAINER && !b->empty()) {
                return;
            }
            chunks.addItem(std::make_shared<Item>(*item_library(b->uid)), 1, t);
            tile_library[t.x][t.y].building = nullptr;
            chunks.eraseBuilding(t);
        }
    }
}

void World::plantCrop(std::shared_ptr<Item> item)
{
    // no need for a redundant check of activeTile, as that is handled in ::interact()
    sf::Vector2i t = *activeTile;
    Floor* f = chunks.floor(*activeTile);
    if (plantableTile(t) && (f->type == Floor_Type::TILLED
    || f->type == Floor_Type::WATERED)
    && !f->planted) {
        item->take(1);
        f->planted = true;

        Crop* c = crop_library.get(item->getUID());
        if (c) {
            crops[t.x][t.y] = Crop(*c);
            crops[t.x][t.y].place(t, f->getPosition());
        }
        tileToLibrary(t);
    }

}

bool World::changeActiveTile(Floor_Type prereq, Floor_Type ntype)
{
    bool change = false;

    if (activeTile) {
        Floor* f = chunks.floor(*activeTile);
        change = (f->type == prereq);
        if (change) {
            f->setType(ntype);
            tileToLibrary(f);
        }
    }

    return change;
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

    chunks.check(player_coordinates);
}

void World::setInteracting(bool interacting)
{
    this->interacting = interacting;
}

Floor* World::activeFloor(sf::Vector2i i)
{
    Floor* f = nullptr;

    if (activeTile) {
        f = chunks.floor(*activeTile);
    }

    return f;
}

void World::checkPickup(Player_Inventory& inventory, Player& player)
{
    chunks.checkPickup(inventory, player.getPosition(), pickup_all);
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

void World::setActiveBuilding()
{
    if (activeTile) {
        sf::Vector2i t = *activeTile;
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
            if (tile_library[i.x + x][i.y + y].tree) {
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
    return (!info.tree
            && !info.rock
            && !info.building
            && info.detail != Detail_Type::WATER
            && info.detail != Detail_Type::LAVA
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
    return (info.floor == Floor_Type::TILLED || info.floor == Floor_Type::TILLED);
}

bool World::passableTile(sf::Vector2i i)
{
    return passableTile(tile_library[i.x][i.y]);
}

bool World::passableTile(Floor_Info& info)
{
    return (emptyTile(info));
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(chunks, states);

    for (const auto& r : crops) {
        for (const auto& c : r.second) {
            target.draw(c.second, states);
        }
    }

    for (const auto& v : vehicles) {
        target.draw(*v, states);
    }
}
