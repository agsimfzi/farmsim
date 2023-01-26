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

class Game : public State_Hook {
public:
    Game(sf::View& nview);
    void update(float deltaTime);
    void enter();
    Player& getPlayer();
    World& getWorld();

    void clickLeft();
    void releaseLeft();

    void clickRight();
    void releaseRight();

    void escape();

    void stopInput();

    Game_State getState();

    Entity* mousedEntity(sf::Vector2f mpos);

    Player_Inventory& getInventory();

    const Game_Renderer& getRenderer();

    sf::View& getView();

private:
    Item_Library item_library;

    World world { item_library };

    Player player;
    Player_Inventory inventory;

    sf::View& view;

    Game_State state { Game_State::PEACE };

    void prepRenderer();
    Game_Renderer renderer;
};
