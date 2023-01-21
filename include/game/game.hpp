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

#include "game_state.hpp"

class Game : public sf::Drawable, public State_Hook {
public:
    Game(sf::RenderWindow& nwindow, sf::View& nview);
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

    void scroll(float delta);

    Game_State getState();

    Entity* mousedEntity(sf::Vector2f mpos);

    Player_Inventory& getInventory();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    //Texture_Manager textures;

    Item_Library item_library;

    World world{ item_library };

    Player player;
    Player_Inventory inventory;

    sf::RenderWindow& window;
    sf::View& view;

    Game_State state { Game_State::PEACE };

    const static float interactThreshold;
        // distance at which a player can interact with something
        // probably should deprecate with SDV-type aim
};
