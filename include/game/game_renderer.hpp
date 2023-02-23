#pragma once
#include <SFML/Graphics.hpp>

#include <map>
#include <vector>

#include <world/world.hpp>

class Game_Renderer : public sf::Drawable {
public:
    Game_Renderer() = default;
    void clear();
    void load(World& world, Player& player);

private:
    Map_Tile<std::shared_ptr<Floor>> floor;
    Map_Tile<std::shared_ptr<Detail>> details;
    Map_Tile<std::shared_ptr<Tree>> trees;
    Map_Tile<std::shared_ptr<Rock>> rocks;
    Map_Tile<std::shared_ptr<sf::Sprite>> buildings;
    std::vector<std::shared_ptr<Item>> items;

    void placeEntitySprite(int y, sf::Sprite& sprite);
    const static sf::Vector2i renderDistance;
    sf::Vector2i playerCoordinates;
    sf::Vector2i renderStart;
    sf::Vector2i renderEnd;
    std::vector<sf::Sprite> base;
    std::map<int, std::vector<sf::Sprite>> layered;

    std::vector<std::vector<sf::Drawable*>> drawables;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
