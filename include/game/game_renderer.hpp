#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <vector>

#include <world/world.hpp>

class Game_Renderer : public sf::Drawable {
public:
    Game_Renderer() = default;

    void clear();

    void updatePlayerCoordinates(sf::Vector2i coordinates);

    void loadFloor(Map_Tile<Floor>& floor);

    void loadCrops(Map_Tile<Crop>& crops);

    void loadWalls(Map_Tile<Wall>& walls);

    void loadPlayer(Player& player);
    void loadEntities(std::vector<Entity>& entities); // not yet

private:
    void placeEntitySprite(int y, sf::Sprite& sprite);

    const static sf::Vector2i renderDistance;
    sf::Vector2i playerCoordinates;

    sf::Vector2i renderStart;
    sf::Vector2i renderEnd;

    std::vector<sf::Sprite> base;

    std::map<int, std::vector<sf::Sprite>> layered;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
