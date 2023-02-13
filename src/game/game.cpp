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

    giveItemToPlayer("furnace");
    giveItemToPlayer("copper ore", 50);
    giveItemToPlayer("iron ore", 50);
    giveItemToPlayer("gold ore", 50);

    giveItemToPlayer(0);
    giveItemToPlayer(1);
    giveItemToPlayer(3);
    giveItemToPlayer("hammer");

    giveItemToPlayer(100, 25);
    giveItemToPlayer(1000, 50);
    giveItemToPlayer(1001, 50);

    giveItemToPlayer((2));

    giveItemToPlayer(5001);

    giveItemToPlayer(("chest"));

    giveItemToPlayer(121, 50);
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
    world.finalize();
    player.setPosition(world.startPosition());
    view.setCenter(player.getPosition());
}

void Game::clickLeft()
{
    std::shared_ptr<Item> equipped = player_inventory.equippedItem();
    if (equipped) {
        world.useItem(equipped.get());
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
