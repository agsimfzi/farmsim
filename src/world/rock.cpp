#include <world/rock.hpp>

#include <world/tile.hpp>

Rock::Rock(sf::Vector2i coordinates, const sf::Texture& texture)
{
    setTexture(texture);
    sf::Vector2f origin(getGlobalBounds().width / 2, getGlobalBounds().height - (Tile::tile_size / 2.f));
    setOrigin(origin);
    sf::Vector2f pos(coordinates);
    pos *= Tile::tile_size;
    setPosition(pos);
}

bool Rock::dead()
{
    return health <= 0;
}

void Rock::hit(int dmg)
{
    health -= dmg;
}

void Rock::resetHealth()
{
    health = max_health;
}

