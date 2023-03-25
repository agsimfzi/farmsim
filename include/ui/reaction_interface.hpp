#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <item/library.hpp>
#include <item/player_inventory.hpp>
#include <item/reaction.hpp>

#include "scrollable.hpp"
#include "tooltip.hpp"

/// REACTION PANEL ///
///
/// \brief UI element representing an item reaction
///
class Reaction_Panel : public sf::Drawable {
public:
    Reaction_Panel() = delete;

/// FULL CONSTRUCTOR ///
///
/// \brief loads the reaction and places elements
///
    Reaction_Panel(Reaction& rxn, Library& library, sf::Vector2f pos);

    ~Reaction_Panel();

/// isAvailable ///
///
/// \brief returns true if all reagants are available in sufficient quantities to perform the reaction
///
    bool isAvailable();

/// setAvailable ///
///
/// \brief records that the reaction may be performed, and sets the background color to the default
    void setAvailable();

/// unsetAvailable ///
///
/// \brief records that the reaction may NOT be performed, and sets the background color to inventory_bg_unavailable
    void unsetAvailable();

/// contains ///
///
/// \brief returns true if the frame's global bounds contain the passed vector
///
    bool contains(sf::Vector2f mpos);

/// getProduct ///
///
/// \brief returns the product
///
    std::shared_ptr<Item> getProduct();

    std::shared_ptr<Tooltip> tooltip; /**< mouseover for displaying necessary reagants */

private:
    bool available; /**< tracks whether the reaction may be performed */

    std::vector<std::shared_ptr<Item>> reagants; /**< reagant prototypes, with appropriate counts */
    std::shared_ptr<Item> product; /**< product prototype */

    sf::RectangleShape frame;

    const static float outline_thickness; /**< frame outline thickness */

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

/// REACTION INTERFACE ///
///
/// \brief constructs reaction panels from building reactions, for display and access
///
class Reaction_Interface : public sf::Drawable, public Scrollable {
public:

/// DEFAULT CONSTRUCTOR ///
///
/// \brief don't forget to call setView() and setScrollable()!
///
    Reaction_Interface() : Scrollable() {}

/// load ///
///
/// \brief reads a vector of reactions and constructs the panels
///
/// \param reactions From a building (including the null_crafting for inventory access outside a building with reactions or recipes)
/// \param inventory The player's inventory
/// \param library For adding item prototypes to the reaction panels
/// \param pos The top-left position of the interface
    void load(std::vector<Reaction> reactions
            , Player_Inventory& inventory
            , Library& library
            , sf::Vector2f pos);

/// check ///
///
/// \brief reads items to determine reactions are available
///
/// \param inventory The player's inventory, eventually hopefully including building inventories?
///
    void check(Player_Inventory& inventory);

/// close ///
///
/// \brief clears the reaction panels and resets the scrollable
///
    void close();

/// click ///
///
/// \brief returns clicked reaction, along with a prototype of its product. nullptrs are returned if no element is clicked.
///
/// \param mpos Click position, local to the viewport
///
    std::pair<Reaction*, std::shared_ptr<Item>> click(sf::Vector2f mpos);

/// findTooltip ///
///
/// \brief returns moused panel's tooltip, or a nullptr if no panels are moused
    std::shared_ptr<Tooltip> findTooltip(sf::Vector2f mpos);

private:
    std::vector<Reaction> reactions; /**< a list of available reactions */
    std::vector<Reaction_Panel> panels; /**< reaction panels. the indices of this vector correspond to the reactions vector */
    sf::RectangleShape frame;

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
