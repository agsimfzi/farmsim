#pragma once

#include <SFML/Graphics.hpp>

enum class Floor_Type {
    DIRT = 0,
    GRASS,
    TILLED,
    WATERED
};

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

class Floor : public Tile {
public:
    Floor(sf::Vector2i ncoord, const sf::Texture& texture);

    Floor_Type type;

    bool planted = false;

    void setType(Floor_Type ntype);
};

class Detail : public Floor {
public:
    Detail(sf::Vector2i ncoord, const sf::Texture& texture, bool ntiled);

    bool autotiled { false };
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
    bool opened{ false };
    bool locked{ false };

    sf::Vector2f closePos;
    sf::Vector2f openPos;
};
