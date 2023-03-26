#pragma once

#include <entity/player.hpp>

#include <item/library.hpp>

#include <world/world.hpp>

/// NAME ///
///
/// \brief
///
/// \param
///
/// \return
///

/// INTERACT ///
///
/// \brief Performs right-click interactions with the world.
///
class Interact {
public:

/// FULL CONSTRUCTOR ///
/// \brief loads references and stores item use functions in the map
///
/// \param world A reference to the world, for changing the world (and adding items to it)
/// \param player A reference to the player, for reading/changing energy
/// \param item_library A reference to the item library, for creating new items
///
    Interact(World& world, Player& player, Player_Inventory& inventory, Library& library);

/// update ///
/// \brief Attempts to perform the interaction.
///
    void update();

/// start ///
/// \brief Sets active flag to true.
///
    void start();

/// stop ///
/// \brief Sets active flag to false.
///
    void stop();

private:
    bool active = false;

    World& world; /**< for effecting tile changes and item additions */
    Player& player; /**< for tracking/changing player energy */
    Player_Inventory& inventory; /**< for giving items to the player */
    sf::Vector2i* target = nullptr; /**< tracks the currently moused tile */

    Library& library; /**< store prototypes for items, buildings, crops, and vehicles. */

    Map_Tile<Tile>& tile_library; /**< stores tile information. reference to the world-level tile library */
    Map_Tile<Crop>& crops;
    std::vector<std::shared_ptr<Machine>>& machines; /**< for adding machines. reference to world-level machines */
    std::vector<std::shared_ptr<Vehicle>>& vehicles; /**< for adding vehicles. reference to world-level vehicles */

/// interact ///
///
/// \brief Examines the target tile and performs an appropriate interaction
///
    void interact();

/// dismount ///
///
/// \brief Exits a vehicle, if appropriate
///
/// \return true if the operation was performed
    bool dismount(Tile& info);

/// mount ///
///
/// \brief Enters a vehicle, if appropriate
///
/// \return true if the operation was performed
    bool mount(Tile& info);

/// machine ///
///
/// \brief Retrieves products from machine, if appropriate
///
/// \return true if the operation was performed
    bool machine(Tile& info);

/// harvest ///
///
/// \brief Retrieves products from machine, if appropriate
///
/// \return true if the operation was performed
    bool harvest(Tile& info);
};
