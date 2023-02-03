#include <world/tree.hpp>

#include <world/tile.hpp>

Tree::Tree(sf::Vector2i coordinates, const sf::Texture& texture)
{
    setTexture(texture);
    sf::Vector2f origin(getGlobalBounds().width / 2, getGlobalBounds().height - (Tile::tileSize / 2.f));
    setOrigin(origin);
    sf::Vector2f pos(coordinates);
    pos *= Tile::tileSize;
    setPosition(pos);
}

bool Tree::dead()
{
    return health <= 0;
}

void Tree::hit(int dmg)
{
    health -= dmg;
}

void Tree::resetHealth()
{
    health = max_health;
}
