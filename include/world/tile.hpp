#pragma once

#include <deque>

#include <SFML/Graphics.hpp>

class Tile : public sf::Sprite {
public:
    //Tile(){}
    Tile(sf::Vector2i ncoord, const sf::Texture& texture);

    const static float tileSize;
    sf::Vector2i coordinates;
};

class Wall : public Tile {
public:
    Wall(sf::Vector2i ncoord, const sf::Texture& texture);
};

enum class Detail_Type {
    GRASS,
    NULL_TYPE
};

class Detail : public sf::Sprite {
public:
    Detail(Detail_Type type, const sf::Texture& texture);

    Detail_Type type;
};

enum class Floor_Type {
    DIRT = 0,
    TILLED,
    WATERED,
    WATER
};

class Floor : public Tile {
public:
    Floor(sf::Vector2i ncoord, const sf::Texture& texture);

    Floor_Type type;

    bool planted = false;

    std::deque<Detail> details;

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
