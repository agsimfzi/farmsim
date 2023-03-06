#include <world/rock.hpp>

#include <world/tile.hpp>

#include <util/prng.hpp>

Rock::Rock(sf::Vector2i coordinates, const sf::Texture& texture, const Rock::Type type, Rock_Data d)
    : Rock_Data(d)
    , type{ type }
{
    sprite.setTexture(texture);
    sf::Vector2i tpos;
    sf::Vector2i tsize(48, 64);
    tpos.x = static_cast<int>(type) * tsize.x;
    tpos.y = 0; // make this a random factor of tsize.y later (when variants are added)
    sprite.setTextureRect(sf::IntRect(tpos, tsize));
    sf::Vector2f origin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height - (Tile::tile_size / 2.f));
    sprite.setOrigin(origin);
    sf::Vector2f pos(coordinates);
    pos *= Tile::tile_size;
    sprite.setPosition(pos);
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

std::string Rock::product()
{
    return m_product;
}

unsigned int Rock::quantity()
{
    return prng::number(quantity_min, quantity_max);
}

std::string Rock::getName()
{
    return name;
}

Rock::Type Rock::stringToType(std::string s)
{
    if (s == "GRANITE") {
        return Rock::GRANITE;
    }
    else if (s == "LIMESTONE") {
        return Rock::LIMESTONE;
    }
    else if (s == "BASALT") {
        return Rock::BASALT;
    }
    else if (s == "COPPER") {
        return Rock::COPPER;
    }
    else if (s == "IRON") {
        return Rock::IRON;
    }
    else if (s == "GOLD") {
        return Rock::GOLD;
    }
    else {
        return Rock::NULL_TYPE;
    }
}

