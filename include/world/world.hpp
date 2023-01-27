#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <variant>

#include <entity/player.hpp>

#include <item/item.hpp>
#include <item/item_library.hpp>
#include <item/player_inventory.hpp>

#include "crop.hpp"
#include "crop_library.hpp"
#include "tile.hpp"

template <class T>
using Map_Tile = std::map<int, std::map<int, std::unique_ptr<T>>>;

//#define Map_Tile(x) std::map<int, std::map<int, std::unique_ptr<x>>>

/////////////////////////////////////////////////////////////
/// \brief
///
class World {
public:
    World(Item_Library& item_library);

    void newLevel();

    Map_Tile<Floor>& getFloor();
    Map_Tile<Wall>& getWalls();
    Map_Tile<Crop>& getCrops();
    Tile* getWall(int x, int y);

    void erase();
    void makeFloor();
    void makeWater();
    void makeGrass();
    void makeWalls();

    std::vector<sf::FloatRect> getLocalImpassableTiles(sf::Vector2i p);

    void reset();

    void update(Player_Inventory& inventory);

    sf::Vector2i* checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords);

    void useItem(Item* item);

    void useTool(Item* item);
    void hoe();
    void water();

    void plantCrop(Item* item);

    void tick();

    void setInteracting(bool interacting);

    void interact(Player_Inventory& inventory);

    sf::Vector2i posToCoords(sf::Vector2f pos);

private:
    bool changeActiveTile(Floor_Type prereq, Floor_Type ntype);

    Item_Library& item_library;
    Crop_Library crop_library;

    Map_Tile<Wall> walls;
    Map_Tile<Floor> floor;

    Map_Tile<Detail> details;

    Map_Tile<Door> doors;

    sf::Texture& textureFloors;
    sf::Texture& textureWalls;

    sf::Texture& textureDetails;
    sf::Texture& textureTiledDetail;

    bool interacting = false;

    Map_Tile<Crop> crops;

    bool inRange(sf::Vector2i c1, sf::Vector2i c2);

    sf::Clock tickClock;

    sf::Vector2i worldMin { -128, -128 };
    sf::Vector2i worldMax { 128, 128 };

    std::unique_ptr<sf::Vector2i> activeTile;

    void updateAutotiledDetails(sf::Vector2i start, sf::Vector2i end);
    int autotileX(sf::Vector2i i, std::variant<Floor_Type, Detail_Type> type);
    int autotileX(bool n, bool w, bool s, bool e);
    bool adjacentDetailMatch(sf::Vector2i i, Detail_Type type);
    bool adjacentFloorMatch(sf::Vector2i i, Floor_Type type);
};
