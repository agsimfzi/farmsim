#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <item/item.hpp>

#include "biome.hpp"
#include "rock.hpp"
#include "tile.hpp"
#include "tree.hpp"

class Chunk {
public:
    Chunk() = default;
    Chunk(sf::Vector2i start , sf::Vector2i size , Map_Tile<Floor_Info>& info);

    bool contains(sf::Vector2f pos);
    bool contains(sf::Vector2i coords);

    Floor* getFloor(sf::Vector2i i);
    Detail* getDetail(sf::Vector2i i);
    Tree* getTree(sf::Vector2i i);
    Rock* getRock(sf::Vector2i i);
    sf::Sprite* getBuilding(sf::Vector2i i);

    void eraseDetail(sf::Vector2i i);
    void eraseTree(sf::Vector2i i);
    void eraseRock(sf::Vector2i i);
    void eraseBuilding(sf::Vector2i i);

    std::vector<std::shared_ptr<Item>>& getItems();

    void addBuilding(Building* b, sf::Vector2i c);
    void addItem(std::shared_ptr<Item> item, sf::Vector2f pos);

    const sf::Vector2i start;
    const sf::Vector2i end;

    Map_Tile<std::shared_ptr<Floor>>& getFloor();
    Map_Tile<std::shared_ptr<Detail>>& getDetails();
    Map_Tile<std::shared_ptr<Tree>>& getTrees();
    Map_Tile<std::shared_ptr<Rock>>& getRocks();
    Map_Tile<std::shared_ptr<sf::Sprite>>& getBuildings();

    sf::RectangleShape& getFrame();

private:
    sf::FloatRect f_bounds;
    sf::IntRect i_bounds;
    sf::RectangleShape frame;
    Map_Tile<std::shared_ptr<Floor>> floor;
    Map_Tile<std::shared_ptr<Detail>> details;
    Map_Tile<std::shared_ptr<Tree>> trees;
    Map_Tile<std::shared_ptr<Rock>> rocks;
    Map_Tile<std::shared_ptr<sf::Sprite>> buildings;
    std::vector<std::shared_ptr<Item>> items;
};
