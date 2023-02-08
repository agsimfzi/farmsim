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
    sf::Vector2i size(64, 64);
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
    if (tickClock.getElapsedTime().asSeconds() >= 0.1f) {
        tickClock.restart();
        tick();
        chunks.check(player.getCoordinates(Tile::tileSize));
        for (auto& b : buildings) {
            b->tick(item_library);
        }
    }

    if (interacting) {
        interact(player_inventory);
    }

    checkPickup(player_inventory, player);
}

void World::interact(Player_Inventory& player_inventory)
{
    if (activeTile) {
        sf::Vector2i t = *activeTile;
        Floor* f = chunks.floor(*activeTile);
        Floor_Info& info = tile_library[t.x][t.y];
        if (f) {
            Item* i = player_inventory.equippedItem();
            if (info.building && info.building->type != Building::CONTAINER) { // PICKUP PRODUCTS
                Item* p = info.building->activeProduct();
                if (p) {
                    player_inventory.addItem(p, p->count());
                    info.building->clearProduct();
                }
            } // END PICKUP PRODUCTS
            else if (f->planted) { // HARVEST
                if (!crops[t.x].contains(t.y)) {
                    std::cout << "FAILED TO FIND CROP AT TILE " << t << '\n';
                }
                else if (crops[t.x][t.y].fullyGrown()) {
                    player_inventory.addItem(item_library.item(crops[t.x][t.y].harvestUID()));
                    crops[t.x].erase(t.y);
                    f->setType(Floor_Type::TILLED);
                    f->planted = false;
                }
            } // END HARVEST
            else if (i) { // VALID ITEM
                if (i->getType() == Item_Type::SEED && plantableTile(info)) { // SEED
                    plantCrop(i);
                }
                if (f->detail == Detail_Type::WATER && i->getUID() == 1) { // watering can
                    i->resetUses();
                    player_inventory.changed = true;
                }
                else if (i->getType() == Item_Type::BUILDING && buildableTile(info)) { // PLACE BUILDING
                    info.building = std::make_shared<Building>(*building_library(i->getUID()));
                    chunks.addBuilding(i->getUID(), t);
                    player_inventory.takeEquipped();
                    buildings.push_back(info.building);
                }
                else if (info.building && info.building->validReagant(i->getName())) { // ADD REAGANT
                    if (!info.building->activeReagant()) {
                        std::cout << "reagants are empty\n";
                        info.building->setReagant(i);
                        player_inventory.takeEquipped(-1);
                        player_inventory.changed = true;
                        info.building->checkReaction();
                    }
                    else if (info.building->activeReagant()->getName() == i->getName()) {
                        std::cout << "reagant present, count from " << info.building->activeReagant()->count() << " to ";
                        info.building->activeReagant()->add(1);
                        player_inventory.takeEquipped(1);
                        player_inventory.changed = true;
                        std::cout << info.building->activeReagant()->count() << '\n';
                    }
                } // ADD REAGANT
            } // END VALID ITEM
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
            if (info.biome == Biome::OCEAN || info.biome == Biome::LAKE || info.biome == Biome::RIVER) {
                info.floor = Floor_Type::SAND;
                info.detail = Detail_Type::WATER;
            }
            else if (info.biome == Biome::BEACH) {
                info.floor = Floor_Type::SAND;
            }
            else {
                info.floor = Floor_Type::DIRT;

                if (prng::boolean(0.001f)) {
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

void World::finalize(sf::Vector2i player_coordinates)
{
    chunks.check(player_coordinates);
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
    return (validLibraryTile(i.x, i.y) && tile_library[i.x][i.y].detail == type);
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
    std::vector<sf::FloatRect> tiles;

    const static int depth = 1;

    for (int x = p.x - depth; x <= p.x + depth; ++x) {
        for (int y = p.y - depth; y <= p.y + depth; ++y) {
            Tile* t = nullptr;// getWall(x, y);
            if (t != nullptr) {
                tiles.push_back(t->getGlobalBounds());
            }
            else {
                sf::Vector2i c(x, y);
                Floor* f = chunks.floor(c);
                if (f && !passableTile(c)) {
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
        default:
            break;
    }
}

void World::useTool(Item* item)
{
    if (activeTile) {
        switch (item->getUID()) {
            default:
                break;
            case 0: // hoe
                hoe();
                break;
            case 1: // watering can
                if (item->usePercent() > 0) {
                    water();
                    item->reduceUses();
                }
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

void World::water()
{
    changeActiveTile(Floor_Type::TILLED, Floor_Type::WATERED);
}

void World::axe(int factor)
{
    sf::Vector2i t = *activeTile;
    if (tile_library[t.x][t.y].tree) {
        Tree* tree = chunks.tree(t);
        if (tree) {
            tree->hit(factor);
            if (tree->dead()) {
                // create logs on ground
                // create stump
                tile_library[t.x][t.y].tree = false;
                chunks.eraseTree(t);
                Item* item = item_library.item("wood");
                size_t count = prng::number(7, 13);
                chunks.addItem(item, count, t);
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
                    Item* item = item_library.item("stone");
                    size_t count = prng::number(2, 4);
                    chunks.addItem(item, count, t);
                    item = nullptr;
                    unsigned int ore_roll = prng::number(0u, 100u);
                    if (ore_roll < 12) {
                        item = item_library.item("copper ore");
                        count = prng::number(3, 4);
                    }
                    else if (ore_roll < 18) {
                        item = item_library.item("iron ore");
                        count = prng::number(2, 3);
                    }
                    else if (ore_roll < 21) {
                        item = item_library.item("gold ore");
                        count = prng::number(1, 2);
                    }
                    if (item) {
                        chunks.addItem(item, count, t);
                    }
                }
            }
        }
        // check for rocks or whatever
    }
}

void World::hammer()
{
    if (activeTile) {
        sf::Vector2i t = *activeTile;
        Building* b = tile_library[t.x][t.y].building.get();
        if (b) {
            chunks.addItem(item_library(b->uid), 1, t);
            tile_library[t.x][t.y].building = nullptr;
            chunks.eraseBuilding(t);
        }
    }
}

void World::plantCrop(Item* item)
{
    // no need for a redundant check of activeTile, as that is handled in ::interact()
    sf::Vector2i t = *activeTile;
    Floor* f = chunks.floor(*activeTile);
    if ((f->type == Floor_Type::TILLED
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

void World::tick()
{
    for (auto& x : crops) {
        for (auto& y : x.second) {
            sf::Vector2i c = y.second.getCoordinates();
            y.second.tick(chunks.floor(c)->type == Floor_Type::WATERED);
        }
    }
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
    return (!info.tree && !info.rock && !info.building);
}

bool World::buildableTile(sf::Vector2i i)
{
    return buildableTile(tile_library[i.x][i.y]);
}

bool World::buildableTile(Floor_Info& info)
{
    return (emptyTile(info) && info.detail != Detail_Type::WATER && !plantableTile(info));
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
    return (emptyTile(info) && info.detail != Detail_Type::WATER);
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(chunks, states);

    for (const auto& r : crops) {
        for (const auto& c : r.second) {
            target.draw(c.second, states);
        }
    }
}
