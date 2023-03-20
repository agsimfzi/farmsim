#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <vector>

#include <ai/ai.hpp>

#include <entity/player.hpp>

#include <item/item_library.hpp>
#include <item/player_inventory.hpp>

#include <resources/texture_manager.hpp>

#include <system/state_hook.hpp>

#include <world/world.hpp>

#include "game_renderer.hpp"
#include "game_state.hpp"
#include "use_item.hpp"

class Game : private State_Hook, public sf::Drawable {
public:
    Game(sf::View& nview);
    void update(float deltaTime);
    void prepRenderer();
    void enter();
    Player& getPlayer();
    World& getWorld();

    void changeSeason(); // effects state change
    void nextSeason();   // performs pertinent operations

    void startGame();

    void clickLeft();
    void releaseLeft();

    void clickRight();
    void releaseRight();

    void escape();

    void stopInput();

    Game_State getState();

    Entity* mousedEntity(sf::Vector2f mpos);

    Player_Inventory& getInventory();

    sf::View& getView();

    void giveItemToPlayer(std::string name, size_t count = 1);
    void giveItemToPlayer(size_t uid, size_t count = 1);

    int playerEnergy();
    int maxPlayerEnergy();
    float energyPercent();

    Item_Library& getItemLibrary();

    void tick();

private:
    Item_Library item_library;

    World world { item_library };

    Player player;
    Player_Inventory player_inventory;

    sf::View& view;

    Game_State state { Game_State::STANDARD };

    sf::Clock tick_clock;

    Game_Renderer renderer;

    Use_Item use_item{ world, player, item_library };

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
