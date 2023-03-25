#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <item/item.hpp>

#include "biome.hpp"
#include "rock.hpp"
#include "tile.hpp"
#include "tree.hpp"

#include "season.hpp"

/// CHUNK ///
///
/// \brief a discrete block of world tiles for batching world load and unload operations
///
class Chunk {
public:
    Chunk() = default;

/// FULL CONSTRUCTOR ///
///
/// \brief reads the passed tiles into a new chunk
///
    Chunk(sf::Vector2i start, sf::Vector2i size, Map_Tile<Floor_Info>& info, Season s);

/// update ///
///
/// \brief updates animated world sprites
///
    void update();

/// contains ///
///
/// \brief returns true if the chunk contains the passed world position
///
    bool contains(sf::Vector2f pos);

/// contains ///
///
/// \brief returns true if the passed tile coordinates are inside the chunk
///
    bool contains(sf::Vector2i coords);

/// getFloor ///
///
/// \brief returns a pointer to the indexed floor
///
    Floor* getFloor(sf::Vector2i i);

/// getDetail ///
///
/// \brief returns a pointer to the indexed detail
///
    Detail* getDetail(sf::Vector2i i);

/// getTree ///
///
/// \brief returns a pointer to the indexed tree (to deprecate)
///
    Tree* getTree(sf::Vector2i i);

/// getRock ///
///
/// \brief returns a pointer to the indexed rock sprite (to deprecate, but the sprite-only behavior is good)
///
    sf::Sprite* getRock(sf::Vector2i i);

/// getBuilding ///
///
/// \brief returns a shared pointer to the indexed building
///
    std::shared_ptr<Building> getBuilding(sf::Vector2i i);

/// eraseDetail ///
///
/// \brief removes the indexed detail
///
    void eraseDetail(sf::Vector2i i);

/// eraseTree ///
///
/// \brief removes the indexed tree
///
    void eraseTree(sf::Vector2i i);

/// eraseRock ///
///
/// \brief removes the indexed rock
///
    void eraseRock(sf::Vector2i i);

/// eraseBuilding ///
///
/// \brief removes the indexed building
///
    void eraseBuilding(sf::Vector2i i);

/// getItems ///
///
/// \brief returns local scattered world items
///
    std::vector<std::shared_ptr<Item>>& getItems();

/// addBuilding ///
///
/// \brief adds the passed building at the specified coordinates
///
    void addBuilding(std::shared_ptr<Building> b, sf::Vector2i c);

/// addItem ///
///
/// \brief drops the passed item on the ground at the specified position
    void addItem(std::shared_ptr<Item> item, sf::Vector2f pos);

    const sf::Vector2i start; /**< lowest tile index */
    const sf::Vector2i end; /**< highest tile index */

/// getFloor ///
///
/// \brief returns all chunk floor tiles
///
    Map_Tile<std::shared_ptr<Floor>>& getFloor();

/// getDetails ///
///
/// \brief returns all chunk details
///
    Map_Tile<std::shared_ptr<Detail>>& getDetails();

/// getTrees ///
///
/// \brief return all chunk trees (to deprecate)
///
    Map_Tile<std::shared_ptr<Tree>>& getTrees();

/// getRocks ///
///
/// \brief return all chunk rock sprites (to deprecate, but keep sprite-only behavior)
///
    Map_Tile<sf::Sprite>& getRocks();

/// getBuildings ///
///
/// \brief return all chunk buildings
///
    Map_Tile<std::shared_ptr<Building>>& getBuildings();

/// getFrame ///
///
/// \brief returns the global boundaries of the chunk
    sf::RectangleShape& getFrame();

private:
    sf::FloatRect f_bounds; /**< world position chunk boundaries */
    sf::IntRect i_bounds;/**< coordinate boundaries */
    sf::RectangleShape frame; /**< boundary frame around all tiles */
    Map_Tile<std::shared_ptr<Floor>> floor; /**< tiles */
    Map_Tile<std::shared_ptr<Detail>> details; /**< tile details */
    Map_Tile<std::shared_ptr<Tree>> trees; /**< trees (to become part of details) */
    Map_Tile<sf::Sprite> rocks; /**< rocks (to become part of details */
    Map_Tile<std::shared_ptr<Building>> buildings; /**< buildings */
    std::vector<std::shared_ptr<Item>> items; /**< dropped or otherwise loose items */
};
