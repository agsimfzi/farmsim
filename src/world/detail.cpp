#include <world/detail.hpp>

#include <world/tile_size.hpp>

#include <util/prng.hpp>

Detail::Detail(sf::Vector2i coordinates, Detail_Data data)
    : Detail_Data(data)
    , coordinates { coordinates }
{
    type = stringToType(type_string);
}

Detail::Type Detail::stringToType(std::string s)
{
    if (s == "TREE") {
        return Detail::TREE;
    }
    else if (s == "ROCK") {
        return Detail::ROCK;
    }
    else {
        return Detail::NULL_TYPE;
    }
}

std::string Detail::typeToString(Detail::Type t)
{
    switch (t) {
        case TREE:
            return "TREE";
        case ROCK:
            return "ROCK";
        default:
            return "INEXPLICABLE NULL DETAIL TYPE";
    }
}

Detail::Type Detail::getType()
{
    return type;
}

void Detail::setSeasonalTextureRect(Season s)
{
    if (type == TREE) {
        texture_rect.top = static_cast<int>(s) * texture_rect.height;
    }
}

sf::IntRect Detail::getTextureRect()
{
    return texture_rect;
}

bool Detail::dead()
{
    return health <= 0;
}

void Detail::hit(int dmg)
{
    health -= dmg;
}

void Detail::resetHealth()
{
    health = max_health;
}

std::string Detail::getProduct()
{
    return product;
}

unsigned int Detail::quantity()
{
    return prng::number(quantity_min, quantity_max);
}

std::string Detail::getName()
{
    return name;
}
