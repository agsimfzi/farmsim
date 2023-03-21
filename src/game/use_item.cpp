#include <game/use_item.hpp>

#include <util/prng.hpp>

Use_Item::Use_Item(World& world, Player& player, Library& library)
    : world { world }
    , player { player }
    , library { library }
    , tile_library { world.getTileLibrary() }
    , machines { world.getMachines() }
    , vehicles { world.getVehicles() }
{
    use_map[Item_Type::CRAFTING] = std::bind(&Use_Item::crafting, this, std::placeholders::_1);
    use_map[Item_Type::FOOD] = std::bind(&eat, this, std::placeholders::_1)  ;
    use_map[Item_Type::TOOL] = std::bind(&tool, this, std::placeholders::_1);
    use_map[Item_Type::TREASURE] = std::bind(&treasure, this, std::placeholders::_1);
    use_map[Item_Type::SEED] = std::bind(&seed, this, std::placeholders::_1);
    use_map[Item_Type::PLANT] = std::bind(&plant, this, std::placeholders::_1);
    use_map[Item_Type::BUILDING] = std::bind(&building, this, std::placeholders::_1);
    use_map[Item_Type::VEHICLE] = std::bind(&vehicle, this, std::placeholders::_1);
    use_map[Item_Type::RAW_MATERIAL] = std::bind(&rawMaterial, this, std::placeholders::_1);
    use_map[Item_Type::WAND] = std::bind(&wand, this, std::placeholders::_1);
    use_map[Item_Type::NULL_TYPE] = std::bind(&nullType, this, std::placeholders::_1);
}

void Use_Item::update(std::shared_ptr<Item> equipped)
{
    if (!equipped || equipped->count() == 0) {
        stop();
    }
    else if (active && use_timer.getElapsedTime().asSeconds() > use_threshold) {
        use_timer.restart();
        use(equipped);
    }
}

void Use_Item::start(std::shared_ptr<Item> equipped)
{
// set use threshold by item type
    switch (equipped->getType()) {
        case Item_Type::SEED:
            use_threshold = min_threshold;
            break;
        case Item_Type::TOOL:
            use_threshold = tool_threshold;
            break;
        default:
            use_threshold = base_threshold;
    }

    use(equipped);
    active = true;
    use_timer.restart();
}

void Use_Item::stop()
{
    active = false;
}

void Use_Item::use(std::shared_ptr<Item> item)
{
    if (item) {
        if (item->edible()) {
            eat(item);
        }
        else {
            target = world.activeTile();
            if (target) {
                use_map[item->getType()](item);
            }
        }
    }
    else {
        stop();
    }
}

void Use_Item::eat(std::shared_ptr<Item> item)
{
    player.addEnergy(item->useFactor());
    item->take(1);
}

void Use_Item::crafting(std::shared_ptr<Item> item)
{
    // unimplemented and likely never to be
}

void Use_Item::treasure(std::shared_ptr<Item> item)
{
    // unimplemented
}

void Use_Item::seed(std::shared_ptr<Item> item)
{
    Crop crop = library.crop(item->getUID());
    if (world.plantableTile(*target) && crop.checkSeason(world.getSeason())) {
        item->take(1);

        auto unwater = [&](sf::Vector2i c)
        {
            world.getChunks().floor(c)->setType(Floor_Type::TILLED);
            world.tileToLibrary(c);
        };
        crop.unwater = unwater;
        world.addCrop(*target, crop);
    }
}

void Use_Item::plant(std::shared_ptr<Item> item)
{
    // ???
}

void Use_Item::building(std::shared_ptr<Item> item)
{
    Floor_Info& info = tile_library[target->x][target->y];
    if (world.buildableTile(info)) {
        info.building = library.building(item->getUID());
        world.getChunks().addBuilding(info.building, *target);
        item->take(1);
        if (info.building->type == Building::MACHINE) {
            machines.push_back(std::dynamic_pointer_cast<Machine>(info.building));
        }
    }
}

void Use_Item::vehicle(std::shared_ptr<Item> item)
{
    switch (item->getUID()) {
        default:
            break;
        case 10000: // boat
            if (tile_library[target->x][target->y].floor == Floor_Type::WATER) {
                sf::Vector2f pos = world.getChunks().floor(*target)->getPosition();
                vehicles.push_back(std::make_shared<Vehicle>(pos, library.vehicle(Vehicle::BOAT)));
                item->take(1);
            }
            break;
        case 10001: // broom
            if (world.emptyTile(*target)) {
                sf::Vector2f pos = world.getChunks().floor(*target)->getPosition();
                vehicles.push_back(std::make_shared<Vehicle>(pos, library.vehicle(Vehicle::BROOM)));
                item->take(1);
            }
            break;
    }
}

void Use_Item::rawMaterial(std::shared_ptr<Item> item)
{
    Floor_Info& info = tile_library[target->x][target->y];
    if (info.building && info.building->type == Building::MACHINE) {
        auto m = std::dynamic_pointer_cast<Machine>(info.building);
        m->addReagant(item);
    }
}

void Use_Item::wand(std::shared_ptr<Item> item)
{
    // awaiting the magic system
}

void Use_Item::nullType(std::shared_ptr<Item> item)
{
    // this should be logged
}

void Use_Item::tool(std::shared_ptr<Item> item)
{
    if (player.getEnergy() > 0) {
        player.takeEnergy(item->useFactor());
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
                axe(item);
                break;
            case 3: // pick
                pick(item);
                break;
            case 4: // hammer
                hammer(item);
                break;
        }
    }
}

void Use_Item::hoe()
{
    if (!world.changeActiveTile(Floor_Type::GRASS, Floor_Type::DIRT)) {
        if (!world.changeActiveTile(Floor_Type::DIRT, Floor_Type::TILLED)) {
            if (tile_library[target->x][target->y].floor == Floor_Type::TILLED
            || tile_library[target->x][target->y].floor == Floor_Type::WATERED) {
                world.removeCrop(*target);
            }
        }
    }
    else {
        world.autotile(*target - sf::Vector2i(1, 1), *target + sf::Vector2i(1, 1), Floor_Type::GRASS);
    }
}

void Use_Item::water(std::shared_ptr<Item> item)
{
    if (tile_library[target->x][target->y].floor == Floor_Type::WATER) {
        item->resetUses();
    }
    else if (item->usePercent() > 0) {
        item->reduceUses();
        world.changeActiveTile(Floor_Type::TILLED, Floor_Type::WATERED);
    }
}

void Use_Item::axe(std::shared_ptr<Item> item)
{
    Floor_Info& info = tile_library[target->x][target->y];
    if (info.tree != Tree::Type::NULL_TYPE) {
        Tree* tree = world.getChunks().tree(*target);
        tree->hit(item->useFactor());
        if (tree->dead()) {
            // create logs on ground
            // create stump
            info.tree = Tree::Type::NULL_TYPE;
            world.getChunks().eraseTree(*target);
            std::string type = tree->typeToString(tree->type);
            size_t count = prng::number(7, 13);
            std::shared_ptr<Item> item = library.item(type + " wood");
            item->setCount(count);
            world.getChunks().addItem(item, *target);
        }
    }
    else if (info.building) {
        std::shared_ptr<Building> b = info.building;
        if (b->type == Building::LOOTABLE) {
            b->health -= item->useFactor();
            if (b->health <= 0) {
                for (auto& i : b->getInventory().front()) {
                    world.getChunks().addItem(i, *target);
                }
                info.building.reset();
                world.getChunks().eraseBuilding(*target);
            }
        }
    }
}

void Use_Item::pick(std::shared_ptr<Item> item)
{
    if (!world.changeActiveTile(Floor_Type::TILLED, Floor_Type::DIRT)
    && !world.changeActiveTile(Floor_Type::WATERED, Floor_Type::DIRT)) {
        if (tile_library[target->x][target->y].rock) {
            Rock* rock = tile_library[target->x][target->y].rock.get();
            if (rock) {
                rock->hit(item->useFactor());
                if (rock->dead()) {
                    if (prng::boolean()) { // INDEPENDENT COAL-SPAWN CHANCE
                        std::shared_ptr<Item> item = library.item("coal");
                        item->setCount(prng::number(3, 7));
                        world.getChunks().addItem(item, *target);
                    }

                    std::shared_ptr<Item> i = library.item(rock->product());
                    i->setCount(rock->quantity());
                    world.getChunks().addItem(i, *target);

                    tile_library[target->x][target->y].rock = nullptr;
                    world.getChunks().eraseRock(*target);
                }
            }
        }
    }
}

void Use_Item::hammer(std::shared_ptr<Item> item)
{
    Building* b = tile_library[target->x][target->y].building.get();
    if (b) {
        if (!b->destructible  && !b->empty()) {
            return;
        }
        world.getChunks().addItem(library.item(b->uid), *target);
        tile_library[target->x][target->y].building = nullptr;
        world.getChunks().eraseBuilding(*target);
    }
}
