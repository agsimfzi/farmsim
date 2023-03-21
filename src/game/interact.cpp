#include <game/interact.hpp>

Interact::Interact(World& world, Player& player, Player_Inventory& inventory, Library& library)
    : world { world }
    , player { player }
    , inventory { inventory }
    , library { library }
    , tile_library { world.getTileLibrary() }
    , crops { world.getCrops() }
    , machines { world.getMachines() }
    , vehicles { world.getVehicles() }
{}

void Interact::update()
{
    interact();
}

void Interact::start()
{
    active = true;
}

void Interact::stop()
{
    active = false;
}

void Interact::interact()
{
    target = world.activeTile();
    if (target) {
        Floor_Info& info = tile_library[target->x][target->y];
        Floor* floor = world.getChunks().floor(*target);
        if (dismount(info, floor) || mount(info, floor)) {
            stop();
        }
        else if (harvest(info, floor)) {}
        else if (machine(info, floor)) {}
    }
}

bool Interact::dismount(Floor_Info& info, Floor* floor)
{
    bool performed = false;
    if (player.getVehicle()
    && world.emptyTile(info)
    && (world.emptyTile(player.getCoordinates(Tile::tile_size)) || player.getVehicle()->type == Vehicle::BOAT)) {
        vehicles.push_back(player.getVehicle());
        player.setPosition(floor->getPosition());
        player.setVehicle(nullptr);
        performed = true;
    }
    return performed;
}

bool Interact::mount(Floor_Info& info, Floor* floor)
{
    bool performed = false;
    for (auto v = vehicles.begin(); v != vehicles.end();) {
        if ((*v) && floor->getGlobalBounds().contains((*v)->getPosition())) {
            player.setVehicle((*v));
            vehicles.erase(v);
            performed = true;
            break;
        }
        else {
            v++;
        }
    }
    return performed;
}

bool Interact::machine(Floor_Info& info, Floor* floor)
{
    bool performed = false;
    if (info.building && info.building->type == Building::MACHINE) {
        auto m = std::dynamic_pointer_cast<Machine>(info.building);
        inventory.addItem(m->activeProduct());
        performed = true;
    }
    return performed;
}

bool Interact::harvest(Floor_Info& info, Floor* floor)
{
    bool performed = false;
    if (info.planted) {
        Crop& crop = crops[target->x][target->y];
        if (crop.fullyGrown()) {
            std::shared_ptr<Item> i = library.item(crop.harvestUID());
            i->setCount(crop.getQuantity());
            inventory.addItem(i);
            if (i) {
                world.getChunks().addItem(i, player.getCoordinates(Tile::tile_size));
            }
            if (crop.regrows()) {
                crop.harvestRegrowable();
            }
            else {
                world.removeCrop(*target);
            }
        }
    }
    return performed;
}
