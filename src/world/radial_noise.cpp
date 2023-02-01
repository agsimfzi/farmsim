#include <world/radial_noise.hpp>

#include <util/primordial.hpp>

Radial_Noise::Radial_Noise(sf::Vector2i min, sf::Vector2i max, sf::Vector2i center)
    : min{ min }
    , max{ max }
    , center{ center }
{
    size = max - min;
}

double Radial_Noise::get(int x, int y)
{
    sf::Vector2f c(center);
    sf::Vector2f i(x, y);
    sf::Vector2f v = vectorDistance(c, i);
    v.x /= (size.x / 2.f);
    v.y /= (size.y / 2.f);
    double factor = scalarDistance(v);
    //double factor = scalar / ((size.x + size.y) / 4);
    if (factor > 1.d) {
        factor = 1.d;
    }
   return factor;
}

double Radial_Noise::inv(int x, int y)
{
    return (1.d - get(x, y));
}
