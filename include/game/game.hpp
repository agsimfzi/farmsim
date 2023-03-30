#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <vector>

#include <entity/player.hpp>

#include <item/library.hpp>
#include <item/player_inventory.hpp>

#include <resources/texture_manager.hpp>

#include <system/state_hook.hpp>

#include <world/world.hpp>

#include "game_renderer.hpp"
#include "game_state.hpp"
#include "interact.hpp"
#include "use_item.hpp"

/// GAME ///
///
/// \brief Primary class for game logic and resources
///
class Game : private State_Hook, public sf::Drawable {
public:

/// FULL CONSTRUCTOR ///
///
/// \brief loads entities and, for now, gives player debug items
///
    Game(sf::View& nview);

/// prepRenderer ///
///
/// \brief clears and loads the game_renderer
///
    void prepRenderer();

/// getPlayer ///
///
/// \brief returns a reference to the player object
///
    Player& getPlayer();

/// getWorld ///
///
/// \brief returns a reference to the game world
///
    World& getWorld();

/// nextSeason ///
///
/// \brief performs operations for the next season
///
    void nextSeason();   // performs pertinent operations

/// startGame ///
///
/// \brief performs initialization when the game is fully-loaded and ready to play
///
    void startGame();

/// clickLeft ///
///
/// \brief starts using equipped item
///
    void clickLeft();

/// releaseLeft ///
///
/// \brief stops using equipped item
///
    void releaseLeft();

/// clickRight ///
///
/// \brief starts world interaction
///
    void clickRight();

/// releaseRight ///
///
/// \brief stops world interaction
///
    void releaseRight();

/// escape ///
///
/// \brief opens the pause menu
///
    void escape();

/// stopInput ///
///
/// \brief cancels any player input
///
    void stopInput();

/// getState ///
///
/// \brief returns the game state
///
    Game_State getState();

/// mousedEntity ///
///
/// \brief returns a pointer to any moused entity, preferring NPCs
///
    Entity* mousedEntity(sf::Vector2f mpos);

/// getInventory ///
///
/// \brief returns a reference to the player's inventory
///
    Player_Inventory& getInventory();

/// getView ///
///
/// \brief returns a reference to the game viewport
///
    sf::View& getView();

/// giveItemToPlayer ///
///
/// \brief retrieves item of passed name from the library and adds it to the player's inventory
///
    void giveItemToPlayer(std::string name, size_t count = 1);

/// giveItemToPlayer ///
///
/// \brief retrieves item of passed UID from the library and adds it to the player's inventory
///
    void giveItemToPlayer(size_t uid, size_t count = 1);

/// playerEnergy ///
///
/// \brief returns the player's current energy
///
    int playerEnergy();

/// maxPlayerEnergy ///
///
/// \brief returns the player's max energy
///
    int maxPlayerEnergy();

/// energyPercent ///
///
/// \brief returns the player's current energy as a decimal percent of max
///
    float energyPercent();

/// getLibrary ///
///
/// \brief returns a reference to the prototype library
///
    Library& getLibrary();

/// tick ///
///
/// \brief performs tickwise operations for entities and the world
///
    void tick();

    const bool& inputEnabled();
    void enableInput();
    void disableInput();

    void stopUse();

    std::function<void(float)> update;

    void setState(Game_State state);

    std::function<void()> hideUI;
    std::function<void()> showUI;

private:
    Library library; /**< prototype library for items, buildings, crops, and vehicles */

    World world { library }; /**< game world, holding the terrain and buildings */

    Player player; /**< player entity object */
    Player_Inventory player_inventory; /**< ... player inventory. deprecate this. */

    sf::View& view; /**< game's viewport, owned by Shell */

    Game_State state { Game_State::PLAY }; /**< game state. currently unimplemented, but expected to be used for season change */

    sf::Clock tick_clock; /**< time since last tick */
    const static float tick_threshold; /**< time between ticks */

    Game_Renderer renderer; /**< class for drawing the game */

    Use_Item use_item{ world, player, library }; /**< uses the player's equipped item */
    Interact interact{ world, player, player_inventory, library }; /**< interacts with the world (for harvesting, conversation, etc) */

    bool input_enabled = false;

/// changeSeason ///
///
/// \brief effects state change for next season
///
/// \see nextSeason
///
    void changeSeason(); // effects state change

/// play ///
///
/// \brief State::PLAY update. performs world and entity updates; and tick, if threshold is reached
///
/// \param \b delta_time Frame time factor
///
    void play(float delta_time);

    void fadeIn(float delta_time);
    void fadeOut(float delta_time);
    bool checkFadeTimer();
    void setFade();

    sf::Clock fade_timer;
    constexpr static float fade_threshold = 0.005f;

    sf::RectangleShape fade;
    uint8_t fade_alpha;

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
