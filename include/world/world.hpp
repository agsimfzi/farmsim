#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>

#include <entity/player.hpp>

#include <item/item.hpp>
#include <item/item_library.hpp>
#include <item/player_inventory.hpp>

#include "crop.hpp"
#include "player_target.hpp"
#include "tile.hpp"

template <class T>
using Map_Tile = std::map<int, std::map<int, std::unique_ptr<T>>>;

//#define Map_Tile(x) std::map<int, std::map<int, std::unique_ptr<x>>>

/////////////////////////////////////////////////////////////
/// \brief
///
class World : public sf::Drawable {
public:
    World(Item_Library& item_library);

    void newLevel();

    Map_Tile<Floor>& getFloor();
    Map_Tile<Wall>& getWalls();
    Tile* getWall(int x, int y);

    void erase();
    void makeFloor();
    void makeWalls();
    void makeDetails();
    void tileAutomata();
    void tileSnake();
    void makeHazards();
    void makeCover();

    std::vector<sf::FloatRect> getLocalWalls(sf::Vector2i p);
    std::vector<sf::FloatRect> getLocalWalls(sf::Vector2f p);

    void reset();

    void update(Player_Inventory& inventory);

    void checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords);

    void useItem(Item* item);

    void useTool(Item* item);
    void hoe();
    void water();

    void plantCrop(Item* item);

    void tick();

    void setInteracting(bool interacting);

    void interact(Player_Inventory& inventory);

private:
    bool changeActiveTile(Floor_Type prereq, Floor_Type ntype);

    Item_Library& item_library;

    Map_Tile<Wall> walls;
    Map_Tile<Floor> floor;

    Map_Tile<Detail> details;

    Map_Tile<Door> doors;

    sf::Texture& textureFloors;
    sf::Texture& textureWalls;

    sf::Texture& textureDetails;
    sf::Texture& textureTiledDetail;

    bool hasOrthogonalFloor(sf::Vector2i v);
    bool hasDiagonalFloor(sf::Vector2i v);

    std::map<int, std::map<int, bool>> floorMap;

    bool interacting = false;

    std::map<int, std::map<int, Crop>> crops;

    Player_Target player_target;
    bool inRange(sf::Vector2i c1, sf::Vector2i c2);

    const static sf::Vector2i renderDistance;

    sf::Clock tickClock;

    sf::Vector2i worldMin { -128, -128 };
    sf::Vector2i worldMax { 128, 128 };

    void updateAutotiledDetails(sf::Vector2i start, sf::Vector2i end);
    int autotileX(sf::Vector2i i, Detail_Type type);
    bool adjacentDetailMatch(sf::Vector2i i, Detail_Type type);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
