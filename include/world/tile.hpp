#pragma once

#include <deque>

#include <SFML/Graphics.hpp>

#include "tile_info.hpp"

template <class T>
using Map_Tile = std::map<int, std::map<int, T>>;

class Tile : public sf::Sprite {
public:
    Tile() = default;
    Tile(sf::Vector2i ncoord, const sf::Texture& texture);

    const static float tileSize;
    sf::Vector2i coordinates;
};

class Wall : public Tile {
public:
    Wall(sf::Vector2i ncoord, const sf::Texture& texture);
};

class Detail : public Tile {
public:
    Detail() = default;
    //Detail(sf::Vector2i ncoord, Detail_Type type, const sf::Texture& texture);
    Detail(sf::Vector2i ncoord, Detail_Type type, const sf::Texture& texture, sf::IntRect texture_rect);


    Detail_Type type;
};

class Floor : public Tile {
public:
    Floor() = default;
    Floor(sf::Vector2i ncoord, const sf::Texture& texture);
    Floor(Floor_Info info, const sf::Texture& texture);

    Floor_Type type{ Floor_Type::NULL_TYPE };

    bool planted = false;

    Detail_Type detail{ Detail_Type::NULL_TYPE };

    void setType(Floor_Type ntype);
};

class Door : public Wall {
public:
    Door(sf::Vector2i ncoord, const sf::Texture& texture, sf::Vector2i openCoord);

    void open();
    void close();
    void toggleOpened();

    void update();

    void setLocked(bool locked);

private:
    bool opened { false };
    bool locked { false };

    sf::Vector2f closePos;
    sf::Vector2f openPos;
};
