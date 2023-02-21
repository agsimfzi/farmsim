#include <game/game.hpp>

#include <system/database.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>
#include <util/prng.hpp>

//////////////////////////////////////////////////////////////

Game::Game(sf::View& nview)
    : view { nview }
{
    player = Player(Database::getPlayerData(), Texture_Manager::get("PLAYER"));

    giveItemToPlayer("axe");
    giveItemToPlayer("pickaxe");
    giveItemToPlayer("furnace");
    giveItemToPlayer("iron bar", 50);
    giveItemToPlayer("copper bar", 10000);
    giveItemToPlayer("gold bar", 50);
    giveItemToPlayer("coal", 500);
    giveItemToPlayer("iron ore", 100);
    giveItemToPlayer("copper ore", 100);
    giveItemToPlayer("gold ore", 100);
    giveItemToPlayer("wood", 100);
    giveItemToPlayer("plank", 100);
    giveItemToPlayer("stone", 100);

    giveItemToPlayer("broom");
}

void Game::giveItemToPlayer(std::string name, size_t count)
{
    Item* i = item_library(name);
    if (i) {
        std::shared_ptr<Item> it = std::make_shared<Item>(*i);
        it->setCount(count);
        player_inventory.addItem(it);
    }
}

void Game::giveItemToPlayer(size_t uid, size_t count)
{
    Item* i = item_library(uid);
    if (i) {
        std::shared_ptr<Item> it = std::make_shared<Item>(*i);
        it->setCount(count);
        player_inventory.addItem(it);
    }
}

void Game::update(float deltaTime)
{
    if (tick_clock.getElapsedTime().asSeconds() >= 0.1f) {
        tick_clock.restart();
        tick();
    }

    player.update();
    std::vector<sf::FloatRect> local_blocks;
    std::vector<std::pair<Floor_Info, sf::FloatRect>> local_tiles;
    size_t n;
    Vehicle::Type v = Vehicle::NULL_TYPE;
    if (player.getVehicle()) {
        v = player.getVehicle()->type;
    }
    sf::Vector2i c = player.getCoordinates(Tile::tileSize);
    switch (v) {
        case Vehicle::NULL_TYPE:
            local_blocks = world.getLocalImpassableTiles(c);
            break;
        case Vehicle::BOAT:
            local_tiles = world.getLocalTiles(c);
            n = local_tiles.size();
            for (size_t i = 0; i < n; i++) {
                if (local_tiles[i].first.detail != Detail_Type::WATER) {
                    local_blocks.push_back(local_tiles[i].second);
                }
            }
            break;
        case Vehicle::BROOM:
            local_tiles = world.getLocalTiles(c);
            n = local_tiles.size();
            for (size_t i = 0; i < n; i++) {
                if (local_tiles[i].first.biome == Biome::NULL_TYPE) {
                    local_blocks.push_back(local_tiles[i].second);
                }
            }
            break;
        default:
            break;
    }
    view.move(player.move(local_blocks, deltaTime));

    player_inventory.update();
    world.update(player_inventory, player);

    view.setCenter(player.getPosition());
}

Player& Game::getPlayer()
{
    return player;
}

void Game::enter()
{
}

World& Game::getWorld()
{
    return world;
}

void Game::startGame()
{
    world.finalize();
    player.setPosition(world.startPosition());
    view.setCenter(player.getPosition());
}

void Game::clickLeft()
{
    std::shared_ptr<Item> equipped = player_inventory.equippedItem();
    if (equipped) {
        if (world.useItem(equipped)) {
            player_inventory.changed = true;
        }
    }
}

void Game::releaseLeft()
{
    // whatever...
}

void Game::clickRight()
{
    world.interact(player, player_inventory);
    // world.setInteracting(true);
}

void Game::releaseRight()
{
    // world.setInteracting(false);
}

void Game::escape()
{
    newMain(Main_State::MENU);
    player.stop();
}

void Game::stopInput()
{
    player.stop();
}

Game_State Game::getState()
{
    return state;
}

Player_Inventory& Game::getInventory()
{
    return player_inventory;
}

Entity* Game::mousedEntity(sf::Vector2f mpos)
{
    if (player.getSprite().getGlobalBounds().contains(mpos)) {
        return &player;
    }

    return nullptr;
}

sf::View& Game::getView()
{
    return view;
}

float Game::energyPercent()
{
    return (static_cast<float>(player.energy) / static_cast<float>(player.max_energy));
}

int Game::playerEnergy()
{
    return player.energy;
}

Item_Library& Game::getItemLibrary()
{
    return item_library;
}

void Game::tick()
{
    player.tick();
    world.tick(player.getCoordinates(Tile::tileSize));
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(world, states);
    target.draw(player, states);
}
