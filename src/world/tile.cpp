#include <world/tile.hpp>

#include <util/primordial.hpp>

const float Tile::tileSize = 64.f;

//////////////////////////////////////////////////////////////

Tile::Tile(sf::Vector2i ncoord, const sf::Texture& texture)
    : coordinates { ncoord }
{
    setOrigin(sf::Vector2f(tileSize / 2.f, tileSize / 2.f));
    setPosition(sf::Vector2f(coordinates) * tileSize);
    setTexture(texture);
    sf::Vector2i pos(0, 0);
    sf::Vector2i size(tileSize, tileSize);
    setTextureRect(sf::IntRect(pos, size));
}

//////////////////////////////////////////////////////////////

Wall::Wall(sf::Vector2i ncoord, const sf::Texture& texture)
    : Tile { ncoord, texture }
{
}

//////////////////////////////////////////////////////////////

Detail::Detail(Detail_Type type, const sf::Texture& texture)
    : sf::Sprite(texture)
    , type{ type }
{}

//////////////////////////////////////////////////////////////

Floor::Floor(sf::Vector2i ncoord, const sf::Texture& texture)
    : Tile { ncoord, texture }
{
}

void Floor::setType(Floor_Type ntype)
{
    if (type != ntype) {
        type = ntype;
        sf::Vector2i pos(0, (static_cast<int>(type)) * roundFloat(tileSize));
        sf::Vector2i size(tileSize, tileSize);
        setTextureRect(sf::IntRect(pos, size));
    }

    details.clear();
}

//////////////////////////////////////////////////////////////

Door::Door(sf::Vector2i ncoord, const sf::Texture& texture, sf::Vector2i openCoord)
    : Wall { ncoord, texture }
    , closePos { getPosition() }
    , openPos { sf::Vector2f(openCoord) * tileSize }
{
    closePos = getPosition();
    openPos = sf::Vector2f(openCoord) * tileSize;
}

void Door::open()
{
    if (!locked) {
        opened = true;
    }
}

void Door::close()
{
    if (!locked) {
        opened = false;
    }
}

void Door::toggleOpened()
{
    if (!locked) {
        if (opened) {
            close();
        }
        else {
            open();
        }
    }
}

void Door::setLocked(bool locked)
{
    this->locked = locked;
    if (locked) {
        //set locked textureRect
    }
    else {
        //set unlocked textureRect
    }
}
