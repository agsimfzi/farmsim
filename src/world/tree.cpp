#include <world/tree.hpp>

#include <world/tile.hpp>

Tree::Tree(sf::Vector2i coordinates, const sf::Texture& texture, Tree::Type type)
    : type { type }
{
    setTexture(texture);
    sf::Vector2f pos(coordinates);
    pos *= Tile::tile_size;
    setPosition(pos);
}

std::string Tree::typeToString(Tree::Type t)
{
    switch (t) {
        case PINE:
            return "PINE";
        case BIRCH:
            return "BIRCH";
        default:
            return "";
    }
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

void Tree::setSeason(Season s)
{
    sf::Vector2i size(64, 96);
    sf::Vector2i pos;
    pos.x = static_cast<int>(type) * size.x;
    pos.y = static_cast<int>(s) * size.y;
    setTextureRect(sf::IntRect(pos, size));
    sf::Vector2f origin(getGlobalBounds().width / 2, getGlobalBounds().height - (Tile::tile_size / 2.f));
    setOrigin(origin);
}
