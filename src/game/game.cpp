#include <game/game.hpp>

#include <system/database.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>
#include <util/prng.hpp>

//////////////////////////////////////////////////////////////

const float Game::interactThreshold = 64.f;

Game::Game(sf::RenderWindow& nwindow, sf::View& nview)
    : window { nwindow }
    , view { nview }
{
    player = Player(Database::getPlayerData(), Texture_Manager::get("PLAYER"));
    player.setPosition(sf::Vector2f(0.f, 0.f));

    inventory.addItem(item_library.item(0));
    inventory.addItem(item_library.item(1));

    inventory.addItem(item_library.item(1000), 10);
    inventory.addItem(item_library.item(1001), 10);
}

void Game::update(float deltaTime)
{

    player.update();
    view.move(player.move(world.getLocalImpassableTiles(player.getCoordinates(Tile::tileSize)), deltaTime));

    inventory.update();

    world.checkMouseTarget(fMouse(window, view), player.getCoordinates(Tile::tileSize));
    world.update(inventory);
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

void Game::clickLeft()
{
    Item* equipped = inventory.equippedItem();
    if (equipped) {
        world.useItem(equipped);
        inventory.changed = true;
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

void Game::scroll(float delta)
{
    if (delta < 0.f) {
        view.zoom(1.5f);
    }
    else if (delta > 0.f) {
        view.zoom(0.5f);
    }
}

Game_State Game::getState()
{
    return state;
}

Player_Inventory& Game::getInventory()
{
    return inventory;
}

Entity* Game::mousedEntity(sf::Vector2f mpos)
{
    if (player.getSprite().getGlobalBounds().contains(mpos)) {
        return &player;
    }

    return nullptr;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(world, states);
    target.draw(player, states);
}
