#pragma once

#include <SFML/System/Vector2.hpp>

#include <cstddef>
#include <map>

/////////////////////////////////////////////////////////////
/// \brief
///
using Automaton_Cells = std::map<int, std::map<int, bool>>;

class Automaton {
public:
    Automaton(size_t iterations, float chance, sf::Vector2i min, sf::Vector2i max, sf::Vector2i padding);
    Automaton_Cells iterate();

private:
    Automaton_Cells cells;

    size_t iterations;

    sf::Vector2i min;
    sf::Vector2i max;
    sf::Vector2i padding;

    size_t countAdjacentActiveCells(int x, int y);

    void print();
};
