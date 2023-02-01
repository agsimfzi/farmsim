#include <world/automaton.hpp>

#include <util/primordial.hpp>
#include <util/prng.hpp>

#include <iostream>

//////////////////////////////////////////////////////////////

Automaton::Automaton(size_t iterations, float chance, sf::Vector2i min, sf::Vector2i max, sf::Vector2i padding)
    : iterations { iterations }
    , chance { chance }
    , min{ min }
    , max{ max }
    , padding{ padding }
{}

void Automaton::loadCells(Automaton_Cells cells)
{
    this->cells = cells;
}

void Automaton::make()
{
    for (int x = min.x - padding.x; x <= max.x + padding.x; x++) {
        for (int y = min.y - padding.y; y <= max.y + padding.y; y++) {
            bool c = !(x < min.x || x > max.x || y < min.y || y > max.y);
            cells[x][y] = prng::boolean(chance);
        }
    }
}

Automaton_Cells Automaton::iterate()
{
    Automaton_Cells newCells = cells;

    auto a = [](int x) {
        return sqrt(0.1f * x) + 0.01f;
    };

    auto t = [](float x) {
        return 0.5 - sqrt(0.02999 * x);
    };

    std::map<size_t, float> adjAdd;
    std::map<size_t, float> adjTake;

    for (unsigned int x = 0; x <= 8; ++x) {
        adjAdd[x] = a(x);
        adjTake[x] = t(x);
    }

    for (unsigned int i = 0; i < iterations; ++i) {
        std::cout << "performing automaton, iteration " << i;
        for (int x = min.x - padding.x; x <= max.x + padding.x; ++x) {
            for (int y = min.y - padding.y; y <= max.y + padding.y; ++y) {
                int count = countAdjacentActiveCells(x, y);
                if (!cells[x][y] && prng::boolean(adjAdd[count])) {
                    newCells[x][y] = true;
                }
                else if (cells[x][y] && prng::boolean(adjTake[count])) {
                    //newCells[x][y] = false;
                }
            }
        }
        cells = newCells;
    }


    return cells;
}

size_t Automaton::countAdjacentActiveCells(int x, int y)
{
    size_t count { 0 };
    for (int ix = x - 1; ix <= x + 1; ++ix) {
        for (int iy = y - 1; iy <= y + 1; ++iy) {
            if (ix == x && iy == y) {
                continue;
            }

            count += (cells.contains(ix) && cells[x].contains(iy) && cells[ix][iy]);
        }
    }

    return count;
}
