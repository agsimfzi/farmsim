#pragma once

#include "automaton.hpp"

class Lake_Generator {
public:
    Lake_Generator(size_t iterations, float chance, sf::Vector2i min, sf::Vector2i max, sf::Vector2i padding);
    Automaton_Cells iterate();

private:
    Automaton_Cells cells;

    size_t iterations;
    float chance;
    sf::Vector2i min;
    sf::Vector2i max;
    sf::Vector2i padding;
    sf::Vector2i size;
    sf::Vector2i center;
    float maxDistance;

    float findChance(int x, int y);
};
