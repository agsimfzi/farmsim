#pragma once

#include <entity/player.hpp>

#include <item/library.hpp>

#include <world/world.hpp>

/// USE_ITEM ///
///
/// \brief Reads an item and applies it to the world, if appropriate.
///

class Use_Item {
public:

/// FULL CONSTRUCTOR ///
/// \brief loads references and stores item use functions in the map
///
/// \param library A reference to the game-level prototype library
///
    Use_Item(World& world, Player& player, Library& library);

/// update ///
/// \brief Uses the item, if ready.
///
/// \param equipped The player's equipped item.
///
    void update(std::shared_ptr<Item> equipped);

/// start ///
/// \brief Sets an active flag, uses the item, and initializes the use threshold by item type.
///
/// \param equipped The player's equipped item
///
    void start(std::shared_ptr<Item> equipped);

/// stop ///
/// \brief
///
    void stop();

private:
    bool active = false; /**< Tracks whether the player is using an item (click based). */

    sf::Clock use_timer; /**< time since last use */

    float use_threshold = 0.5f; /**< threshold timer must reach for next use */

    float min_threshold = 0.001f; /**< timer threshold for seeds */
    float tool_threshold = 1.f; /**< timer threshold for tool use */
    float base_threshold = 0.5f; /**< timer threshold for most items */

    World& world; /**< for effecting tile changes and item additions */
    Player& player; /**< for tracking/changing player energy */
    sf::Vector2i* target = nullptr; /**< tracks the currently moused tile */

    Library& library; /**< store prototypes for items, buildings, crops, and vehicles. */

    Map_Tile<Tile_Info>& tile_library; /**< stores tile information. reference to the world-level tile library */
    std::vector<std::shared_ptr<Machine>>& machines; /**< for adding machines. reference to world-level machines */
    std::vector<std::shared_ptr<Vehicle>>& vehicles; /**< for adding vehicles. reference to world-level vehicles */

    std::map<Item_Type, std::function<void(std::shared_ptr<Item>)>> use_map; /**< stores use functions by item type */

/// use ///
/// \brief
///
/// \return true if the item is used (depending on type)
///
    void use(std::shared_ptr<Item> item);

/// eat ///
/// \brief Eats one item, restoring player energy.
///
/// \return always false
///
    void eat(std::shared_ptr<Item> item);

/// crafting ///
/// \brief unimplemented
///
/// \return always false
///
    void crafting(std::shared_ptr<Item> item);

/// treasure ///
/// \brief unimplemented
///
/// \return always false
///
    void treasure(std::shared_ptr<Item> item);

/// seed ///
/// \brief Plants a crop if in-season and the active tile is tilled.
///
/// \return always false
///
    void seed(std::shared_ptr<Item> item);

/// plant ///
/// \brief unimplemented
///
/// \return always false
///
    void plant(std::shared_ptr<Item> item);

/// building ///
/// \brief Places a building, if the active tile is empty.
///
/// \return always returns false
///
    void building(std::shared_ptr<Item> item);

/// vehicle ///
/// \brief Places a vehicle, if the tile is appropriate for the specific vehicle.
///
/// \return true if used
///
    void vehicle(std::shared_ptr<Item> item);

/// raw material ///
/// \brief Adds the item stack to the moused machine, if it's a valid reagant.
///
/// \return always false
///
    void rawMaterial(std::shared_ptr<Item> item);

/// wand ///
/// \brief unimplemented pending magic mechanic
///
/// \return always false
///
    void wand(std::shared_ptr<Item> item);

/// nullType ///
/// \brief unimplemented
///
/// \return always false
///
    void nullType(std::shared_ptr<Item> item);

/// tool ///
/// \brief Uses an item of type Tool.
///
/// \return always false
///
    void tool(std::shared_ptr<Item> item);

/// hoe ///
/// \brief Converts grass to dirt, or dirt to tilled soil.
///
    void hoe();

/// water ///
/// \brief Converts tilled soil to watered soil, or fills the can (if used on water).
///
    void water(std::shared_ptr<Item> item);

/// axe ///
/// \brief Uses the axe, which destroys lootables or damages trees.
///
    void axe(std::shared_ptr<Item> item);

/// pick ///
/// \brief Uses the pickaxe, which reverts tilled soil or damages rocks.
///
    void pick(std::shared_ptr<Item> item);

/// hammer ///
/// \brief Uses the hammer, which deconstructs and drops buildings.
///
    void hammer(std::shared_ptr<Item> item);
};
