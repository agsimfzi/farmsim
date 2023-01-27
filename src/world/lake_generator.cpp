#include <world/lake_generator.hpp>

#include <util/primordial.hpp>
#include <util/prng.hpp>

Lake_Generator::Lake_Generator(size_t iterations, float chance, sf::Vector2i min, sf::Vector2i max, sf::Vector2i padding)
    : iterations{ iterations }
    , chance { chance }
    , min{ min }
    , max{ max }
    , padding{ padding }
{
    size = max - min;
    center = min + (size / 2);

    maxDistance = scalarDistance(sf::Vector2f(center), sf::Vector2f(min));

    for (int x = min.x - padding.x; x <= max.x + padding.x; x++) {
        for (int y = min.x - padding.y; y <= max.y + padding.y; y++) {
            bool c = !(x < min.x || x > max.x || y < min.y || y > max.y);
            cells[x][y] = (c && prng::boolean(chance));
        }
    }
}

Automaton_Cells Lake_Generator::iterate()
{
    Automaton lake(iterations, chance, min, max, padding);
    lake.loadCells(cells);
    cells = lake.iterate();
    return cells;
}

float Lake_Generator::findChance(int x, int y)
{
    float c = chance;

    float distance = scalarDistance(sf::Vector2f(center), sf::Vector2f(x, y));

    c *= (1.f - distance / maxDistance);

    if (c > 1.f) {
        c = 1.f;
    }

    return c;
}
