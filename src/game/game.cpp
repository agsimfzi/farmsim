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
    player.setPosition(sf::Vector2f(0.f, 0.f));

    // load position from world instead, in startGame()

    player_inventory.addItem(item_library("furnace"));
    player_inventory.addItem(item_library("copper ore"), 50);
    player_inventory.addItem(item_library("iron ore"), 50);
    player_inventory.addItem(item_library("gold ore"), 50);

    player_inventory.addItem(item_library(0));
    player_inventory.addItem(item_library(1));
    player_inventory.addItem(item_library(3));
    player_inventory.addItem(item_library("hammer"));

    player_inventory.addItem(item_library(100), 25);
    player_inventory.addItem(item_library(1000), 50);
    player_inventory.addItem(item_library(1001), 50);

    player_inventory.addItem(item_library(2));

    player_inventory.addItem(item_library(3001));
}

void Game::update(float deltaTime)
{

    player.update();
    view.move(player.move(world.getLocalImpassableTiles(player.getCoordinates(Tile::tileSize)), deltaTime));

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
    player.setPosition(sf::Vector2f(2560.f, 2560.f));
    view.setCenter(player.getPosition());
    world.finalize(player.getCoordinates(Tile::tileSize));
}

void Game::clickLeft()
{
    Item* equipped = player_inventory.equippedItem();
    if (equipped) {
        world.useItem(equipped);
        player_inventory.changed = true;
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

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(world, states);
    target.draw(player, states);
}
