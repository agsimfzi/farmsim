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
        Tile_Info& info = tile_library[target->x][target->y];
        if (dismount(info) || mount(info)) {
            stop();
        }
        else if (harvest(info)) {}
        else if (machine(info)) {}
    }
}

bool Interact::dismount(Tile_Info& info)
{
    bool performed = false;
    if (player.getVehicle()
    && world.emptyTile(info)
    && (world.emptyTile(player.getCoordinates(tile_size)) || player.getVehicle()->type == Vehicle::BOAT)) {
        vehicles.push_back(player.getVehicle());
        player.setPosition(world.getChunks().floor(*target)->getPosition());
        player.setVehicle(nullptr);
        performed = true;
    }
    return performed;
}

bool Interact::mount(Tile_Info& info)
{
    bool performed = false;
    for (auto v = vehicles.begin(); v != vehicles.end();) {
        if ((*v) && world.getChunks().floor(*target)->getGlobalBounds().contains((*v)->getPosition())) {
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

bool Interact::machine(Tile_Info& info)
{
    bool performed = false;
    if (info.building && info.building->type == Building::MACHINE) {
        auto m = std::dynamic_pointer_cast<Machine>(info.building);
        inventory.addItem(m->activeProduct());
        performed = true;
    }
    return performed;
}

bool Interact::harvest(Tile_Info& info)
{
    bool performed = false;
    if (info.planted) {
        Crop& crop = crops[target->x][target->y];
        if (crop.fullyGrown()) {
            std::shared_ptr<Item> i = library.item(crop.harvestUID());
            i->setCount(crop.getQuantity());
            inventory.addItem(i);
            if (i) {
                world.getChunks().addItem(i, player.getCoordinates(tile_size));
            }
            if (crop.regrows()) {
                crop.harvestRegrowable();
            }
            else {
                world.removeCrop(*target);
            }
            performed = true;
            world.getChunks().updateTile(info);
        }
    }
    return performed;
}
