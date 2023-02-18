#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <variant>

#include <entity/player.hpp>

#include <item/building_library.hpp>
#include <item/item.hpp>
#include <item/item_library.hpp>
#include <item/player_inventory.hpp>

#include "crop.hpp"
#include "crop_library.hpp"

#include "biome.hpp"
#include "chunk.hpp"
#include "chunk_loader.hpp"
#include "tile.hpp"
#include "tile_info.hpp"
#include "vehicle.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class World : public sf::Drawable {
public:
    World(Item_Library& item_library);

    void reset();
    void makeBiomes();
    void makeGrass();
    void initialAutotile();
    void finalize();

    Map_Tile<Crop>& getCrops();

    std::vector<sf::FloatRect> getLocalImpassableTiles(sf::Vector2i p);
    std::vector<std::pair<Floor_Info, sf::FloatRect>> getLocalTiles(sf::Vector2i p);

    void update(Player_Inventory& inventory, Player& player);

    sf::Vector2i* checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords);

    bool useItem(Item* item);

    void useTool(Item* item);
    void hoe();
    void water(Item* item);
    void axe(int factor);
    void pick(int factor);
    void hammer();

    void useVehicle(Item* item);

    void plantCrop(Item* item);

    void useBuilding(Item* item);

    void tick(sf::Vector2i player_coordinates);

    void setInteracting(bool interacting);

    void interact(Player_Inventory& inventory);

    sf::Vector2i posToCoords(sf::Vector2f pos);

    sf::Vector2i worldMin();
    sf::Vector2i worldMax();

    Map_Tile<Floor_Info>& getTileLibrary();

    Floor* activeFloor(sf::Vector2i i);

    void checkPickup(Player_Inventory& inventory, Player& player);

    Chunk_Loader& getChunks();

    sf::Vector2f startPosition();

    void pickupAll();
    void stopPickupAll();

    void setActiveBuilding();
    void closeActiveBuilding();
    Building* activeBuilding();

    int energyDiff();

    Building_Library& getBuildingLibrary();

private:
    bool changeActiveTile(Floor_Type prereq, Floor_Type ntype);

    int energy_diff = 0;
    int energy;

    Building* active_building{ nullptr };

    Item_Library& item_library;
    Building_Library building_library;
    Crop_Library crop_library;

    Map_Tile<Floor_Info> tile_library;
    Chunk_Loader chunks { tile_library };

    sf::Vector2i world_min;
    sf::Vector2i world_max;

    bool interacting = false;

    Map_Tile<Crop> crops;

    bool inRange(sf::Vector2i c1, sf::Vector2i c2);

    std::unique_ptr<sf::Vector2i> activeTile;

    std::vector<std::shared_ptr<Machine>> machines;

    std::vector<std::shared_ptr<Vehicle>> vehicles;

    sf::Vector2i start_coords;

    bool pickup_all = false;

    void autotile(sf::Vector2i start, sf::Vector2i end, Detail_Type type);
    int autotileX(sf::Vector2i i, Detail_Type type);
    int autotileX(bool n, bool w, bool s, bool e);
    bool adjacentDetailMatch(sf::Vector2i i, Detail_Type type);
    bool adjacentBiomeMatch(sf::Vector2i i, Biome type);
    bool validLibraryTile(int x, int y);

    void tileToLibrary(sf::Vector2i i);
    void tileToLibrary(Floor* f);

    bool adjacentTree(sf::Vector2i i);

    bool emptyTile(sf::Vector2i i);
    bool emptyTile(Floor_Info& info);

    bool buildableTile(sf::Vector2i i);
    bool plantableTile(sf::Vector2i i);
    bool passableTile(sf::Vector2i i);

    bool buildableTile(Floor_Info& info);
    bool plantableTile(Floor_Info& info);
    bool passableTile(Floor_Info& info);

    void checkBuildings();

    void placeWreckage();

    sf::Vector2i randomNearbyEmptyTile(sf::Vector2i i, int distance = 1);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
