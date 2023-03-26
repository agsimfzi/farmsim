#include <game/game.hpp>

#include <system/database.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>
#include <util/prng.hpp>

//////////////////////////////////////////////////////////////

const float Game::tick_threshold = 0.1f;

Game::Game(sf::View& nview)
    : view { nview }
{
    player = Player(Database::getPlayerData(), Texture_Manager::get("PLAYER"));

    giveItemToPlayer("pine wood", 100);
    giveItemToPlayer("birch wood", 100);

    giveItemToPlayer("coal", 500);
    giveItemToPlayer("copper ore", 100);
    giveItemToPlayer("iron ore", 100);
    giveItemToPlayer("gold ore", 100);

    giveItemToPlayer("pine plank", 100);
    giveItemToPlayer("birch plank", 100);
    giveItemToPlayer("copper bar", 100);
    giveItemToPlayer("iron bar", 100);
    giveItemToPlayer("gold bar", 100);

    giveItemToPlayer("melon", 100);

    giveItemToPlayer("granite", 100);
    giveItemToPlayer("limestone", 100);
    giveItemToPlayer("basalt", 100);

    giveItemToPlayer(1100, 100);
    giveItemToPlayer(1101, 100);
    giveItemToPlayer(1102, 100);

    giveItemToPlayer(1200, 100);
    giveItemToPlayer(1201, 100);
    giveItemToPlayer(1202, 100);

    giveItemToPlayer("bed");
}

void Game::changeSeason()
{
    newMain(Main_State::SEASON_CHANGE);
}

void Game::nextSeason()
{
    player.setMaxEnergy();
    world.nextSeason(player);

    const static size_t ticks = 1000;
    for (size_t i = 0; i < ticks; i++) {
        tick();
    }

    sf::Clock test_timer;

    while (test_timer.getElapsedTime().asSeconds() < 2.f);
}

void Game::giveItemToPlayer(std::string name, size_t count)
{
    std::shared_ptr<Item> i = library.item(name);
    i->setCount(count);
    player_inventory.addItem(i);
}

void Game::giveItemToPlayer(size_t uid, size_t count)
{
    std::shared_ptr<Item> i = library.item(uid);
    i->setCount(count);
    player_inventory.addItem(i);
}

void Game::update(float delta_time)
{
    if (tick_clock.getElapsedTime().asSeconds() >= tick_threshold) {
        tick_clock.restart();
        tick();
    }

    player.update();
    std::vector<sf::FloatRect> local_blocks;
    std::vector<std::pair<Tile_Info, sf::FloatRect>> local_tiles;
    size_t n;
    Vehicle::Type v = Vehicle::NULL_TYPE;
    if (player.getVehicle()) {
        v = player.getVehicle()->type;
    }
    sf::Vector2i c = player.getCoordinates(tile_size);
    switch (v) {
        case Vehicle::NULL_TYPE:
            local_blocks = world.getLocalImpassableTiles(c);
            break;
        case Vehicle::BOAT:
            local_tiles = world.getLocalTiles(c);
            n = local_tiles.size();
            for (size_t i = 0; i < n; i++) {
                if (local_tiles[i].first.floor != Floor_Type::WATER) {
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
    view.move(player.move(local_blocks, delta_time));

    player_inventory.update();
    world.update(player_inventory, player, delta_time);

    view.setCenter(player.getPosition());

    use_item.update(player_inventory.equippedItem());

    prepRenderer();
}

void Game::prepRenderer()
{
    renderer.clear();
    renderer.load(world, player);
}

Player& Game::getPlayer()
{
    return player;
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
    use_item.start(player_inventory.equippedItem());
}

void Game::releaseLeft()
{
    use_item.stop();
}

void Game::clickRight()
{
    interact.start();
}

void Game::releaseRight()
{
    interact.stop();
}

void Game::escape()
{
    newMain(Main_State::MENU);
    stopInput();
}

void Game::stopInput()
{
    player.stop();
    use_item.stop();
    interact.stop();
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
    // check for NPCs BEFORE the player
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
    return (static_cast<float>(player.getEnergy()) / static_cast<float>(player.maxEnergy()));
}

int Game::playerEnergy()
{
    return player.getEnergy();
}

Library& Game::getLibrary()
{
    return library;
}

void Game::tick()
{
    player.tick();
    world.tick(player.getCoordinates(tile_size));
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(renderer, states);
}
