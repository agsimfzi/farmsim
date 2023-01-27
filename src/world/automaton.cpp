#include <world/automaton.hpp>

#include <util/prng.hpp>

#include <iostream>

//////////////////////////////////////////////////////////////

Automaton::Automaton(size_t iterations, float chance, sf::Vector2i min, sf::Vector2i max, sf::Vector2i padding)
    : iterations { iterations }
    , min{ min }
    , max{ max }
    , padding{ padding }
{
    for (int x = min.x - padding.x; x <= max.x + padding.x; x++) {
        for (int y = min.y - padding.y; y <= max.y + padding.y; y++) {
            bool c = !(x < min.x || x > max.x || y < min.y || y > max.y);
            cells[x][y] = (c && prng::boolean(chance));
        }
    }
}

Automaton_Cells Automaton::iterate()
{
    Automaton_Cells newCells = cells;
    //print();

    std::map<size_t, float> adjAdd = { { 0, 0.02 }
                                     , { 1, 0.04 }
                                     , { 2, 0.08 }
                                     , { 3, 0.16 }
                                     , { 4, 0.26 }
                                     , { 5, 0.38 }
                                     , { 6, 0.54 }
                                     , { 7, 0.72 }
                                     , { 8, 0.94 } };

    std::map<size_t, float> adjTake = { { 0, 0.60 }
                                      , { 1, 0.44 }
                                      , { 2, 0.30 }
                                      , { 3, 0.18 }
                                      , { 4, 0.10 }
                                      , { 5, 0.06 }
                                      , { 6, 0.04 }
                                      , { 7, 0.02 }
                                      , { 8, 0.01 } };

    for (unsigned int i = 0; i < iterations; ++i) {
        std::cout << "\tperforming automaton, iteration " << (i + 1) << '\n';
        for (int x = min.x - padding.x; x <= max.x + padding.x; ++x) {
            for (int y = min.y - padding.y; y <= max.y + padding.y; ++y) {
                int count = countAdjacentActiveCells(x, y);
                if (cells[x][y] && prng::boolean(adjTake[count])) {
                    newCells[x][y] = false;
                }
                else if (!cells[x][y] && prng::boolean(adjAdd[count])) {
                    newCells[x][y] = true;
                }
            }
        }
        cells = newCells;
        //print();
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

void Automaton::print()
{
    for (int x = min.x - padding.x; x <= max.x + padding.x; ++x) {
        std::cout << x << '\t';
        int y;
        for (y = min.y - padding.y; y <= max.y + padding.y; ++y) {
            if (cells[x][y]) {
                std::cout << '0';
            }
            else {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
