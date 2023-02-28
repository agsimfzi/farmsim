#include <world/tile.hpp>

#include <util/primordial.hpp>

const float Tile::tile_size = 48.f;

//////////////////////////////////////////////////////////////

Tile::Tile(sf::Vector2i ncoord, const sf::Texture& texture)
    : coordinates { ncoord }
{
    setOrigin(sf::Vector2f(tile_size / 2.f, tile_size / 2.f));
    setPosition(sf::Vector2f(coordinates) * tile_size);
    setTexture(texture);
    sf::Vector2i pos(0, 0);
    sf::Vector2i size(tile_size, tile_size);
    //setTextureRect(sf::IntRect(pos, size));
}

//////////////////////////////////////////////////////////////

Wall::Wall(sf::Vector2i ncoord, const sf::Texture& texture)
    : Tile { ncoord, texture }
{
}

//////////////////////////////////////////////////////////////

Detail::Detail(sf::Vector2i ncoord, Detail_Type type, const sf::Texture& texture, sf::IntRect texture_rect)
    : type{ type }
{
    coordinates = ncoord;
    setOrigin(sf::Vector2f(tile_size / 2.f, tile_size / 2.f));
    setPosition(sf::Vector2f(coordinates) * tile_size);
    setTexture(texture);
    setTextureRect(texture_rect);
}

//////////////////////////////////////////////////////////////

Floor::Floor(sf::Vector2i ncoord, const sf::Texture& texture)
    : Tile { ncoord, texture }
{}

Floor::Floor(Floor_Info info, const sf::Texture& texture)
    : planted{ info.planted }
{
    coordinates = info.coordinates;
    setOrigin(sf::Vector2f(tile_size / 2.f, tile_size / 2.f));
    setPosition(sf::Vector2f(coordinates) * tile_size);
    setTexture(texture);
    sf::Vector2i size(tile_size, tile_size);
    setTextureRect(sf::IntRect(info.texture_pos, size));
    setType(info.floor);
    detail = info.detail;
}

void Floor::setType(Floor_Type ntype)
{
    if (type != ntype) {
        type = ntype;
        sf::Vector2i pos(0, (static_cast<int>(type)) * roundFloat(tile_size));
        sf::Vector2i size(tile_size, tile_size);
        setTextureRect(sf::IntRect(pos, size));
    }
}

//////////////////////////////////////////////////////////////

Door::Door(sf::Vector2i ncoord, const sf::Texture& texture, sf::Vector2i openCoord)
    : Wall { ncoord, texture }
    , closePos { getPosition() }
    , openPos { sf::Vector2f(openCoord) * tile_size }
{
    closePos = getPosition();
    openPos = sf::Vector2f(openCoord) * tile_size;
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
