#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <variant>

#include <entity/player.hpp>

#include <item/library.hpp>
#include <item/player_inventory.hpp>

#include "biome.hpp"
#include "crop.hpp"
#include "chunk.hpp"
#include "chunk_loader.hpp"
#include "season.hpp"
#include "tile.hpp"
#include "tile_info.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class World {
public:
    World(Library& library);

    void reset();
    void makeBiomes();
    void makeGrass();
    void finalize();

    Map_Tile<Crop>& getCrops();

    std::vector<sf::FloatRect> getLocalImpassableTiles(sf::Vector2i p);
    std::vector<std::pair<Floor_Info, sf::FloatRect>> getLocalTiles(sf::Vector2i p);

    void update(Player_Inventory& inventory, Player& player, float deltaTime);

    sf::Vector2i* checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords);

    void tick(sf::Vector2i player_coordinates);

    sf::Vector2i posToCoords(sf::Vector2f pos);

    sf::Vector2i worldMin();
    sf::Vector2i worldMax();

    Map_Tile<Floor_Info>& getTileLibrary();

    Floor* activeFloor(sf::Vector2i i);

    void checkPickup(Player_Inventory& inventory, Player& player, float deltaTime);

    Chunk_Loader& getChunks();

    sf::Vector2f startPosition();

    void pickupAll();
    void stopPickupAll();

    void setActiveBuilding();
    void closeActiveBuilding();
    Building* activeBuilding();

    int energyDiff();

    void nextSeason(Player& player);
    void killUnseasonableCrops();
    void removeCrop(sf::Vector2i i);

    Season getSeason();

    sf::Vector2i* activeTile();

    void addCrop(sf::Vector2i c, Crop crop);

    std::vector<std::shared_ptr<Machine>>& getMachines();
    std::vector<std::shared_ptr<Vehicle>>& getVehicles();

    void tileToLibrary(sf::Vector2i i);
    void tileToLibrary(Floor* f);

    bool emptyTile(sf::Vector2i i);
    bool emptyTile(Floor_Info& info);

    bool buildableTile(sf::Vector2i i);
    bool plantableTile(sf::Vector2i i);
    bool passableTile(sf::Vector2i i);

    bool buildableTile(Floor_Info& info);
    bool plantableTile(Floor_Info& info);
    bool passableTile(Floor_Info& info);

    void autotile(sf::Vector2i start, sf::Vector2i end, Floor_Type type);

    bool changeActiveTile(Floor_Type prereq, Floor_Type ntype);

private:
    Building* active_building{ nullptr };

    Library& library;

    Map_Tile<Floor_Info> tile_library;
    Chunk_Loader chunks { tile_library };

    sf::Vector2i world_min;
    sf::Vector2i world_max;

    Map_Tile<Crop> crops;

    Season season = Season::SPRING;

    bool inRange(sf::Vector2i c1, sf::Vector2i c2);

    std::unique_ptr<sf::Vector2i> active_tile;

    std::vector<std::shared_ptr<Machine>> machines;

    std::vector<std::shared_ptr<Vehicle>> vehicles;

    std::map<Rock::Type, Rock_Data> rock_data;

    sf::Vector2i start_coords;

    bool pickup_all = false;

    int autotileX(sf::Vector2i i, Floor_Type type);
    int autotileX(bool n, bool w, bool s, bool e);

    bool adjacentDetailMatch(sf::Vector2i i, Detail_Type type);
    bool adjacentFloorMatch(sf::Vector2i i, Floor_Type type);
    bool adjacentBiomeMatch(sf::Vector2i i, Biome type);
    bool validLibraryTile(int x, int y);

    bool adjacentTree(sf::Vector2i i);

    void checkBuildings();

    void placeWreckage();

    void eat(std::shared_ptr<Item> item);

    sf::Vector2i randomNearbyEmptyTile(sf::Vector2i i, int distance = 1);

    void seasonalIteration();
};
