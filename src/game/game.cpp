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

    giveItemToPlayer("boat");
/*
    giveItemToPlayer("furnace");
    giveItemToPlayer("copper ore", 50);
    giveItemToPlayer("iron ore", 50);
    giveItemToPlayer("gold ore", 50);

    giveItemToPlayer(100, 25);

    giveItemToPlayer(5001);

    giveItemToPlayer(("chest"));

    giveItemToPlayer(121, 50);

    giveItemToPlayer("crate");
*/
}

void Game::giveItemToPlayer(std::string name, size_t count)
{
    Item i = *item_library(name);
    player_inventory.addItem(std::make_shared<Item>(i), count);
}

void Game::giveItemToPlayer(size_t uid, size_t count)
{
    Item* i = item_library(uid);
    if (i) {
        player_inventory.addItem(std::make_shared<Item>(*i), count);
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
    switch (player.vehicle) {
        case Vehicle::NULL_VEHICLE:
            local_blocks = world.getLocalImpassableTiles(player.getCoordinates(Tile::tileSize));
            break;
        case Vehicle::BOAT:
            local_tiles = world.getLocalTiles(player.getCoordinates(Tile::tileSize));
            n = local_tiles.size();
            for (size_t i = 0; i < n; i++) {
                if (local_tiles[i].first.detail == Detail_Type::WATER) {
                    local_blocks.push_back(local_tiles[i].second);
                }
            }
            break;
        default:
            // brooms can pass anything so nothing needs to be passed!
            break;
    }
    view.move(player.move(local_blocks, deltaTime));
    // view.move(player.move(world.getLocalTiles(player.getCoordinates(Tile::tileSize)), deltaTime));

    player_inventory.update();
    world.update(player_inventory, player);
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
        if (world.useItem(equipped.get())) {
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
    world.setInteracting(true);
}

void Game::releaseRight()
{
    world.setInteracting(false);
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
