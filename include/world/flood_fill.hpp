#pragma once

#include "tile.hpp"

inline void floodFill(int x, int y, Map_Tile<bool>& to_flood, Map_Tile<bool>& flooding)
{
    flooding[x][y] = true;

    if (!flooding[x - 1][y] && to_flood[x - 1][y]) {
        floodFill(x - 1, y, to_flood, flooding);
    }
    if (!flooding[x + 1][y] && to_flood[x + 1][y]) {
        floodFill(x + 1, y, to_flood, flooding);
    }
    if (!flooding[x][y - 1] && to_flood[x][y - 1]) {
        floodFill(x, y - 1, to_flood, flooding);
    }
    if (!flooding[x][y + 1] && to_flood[x][y + 1]) {
        floodFill(x, y + 1, to_flood, flooding);
    }
}

inline void floodCheck(Map_Tile<bool>& to_flood)
{
    Map_Tile<bool> flooding = to_flood;
    for (auto& m : flooding) {
        for (auto& t : m.second) {
            t.second = false;
        }
    }

    int x = flooding.begin()->first;
    int y = flooding.begin()->second.begin()->first;

    floodFill(x, y, to_flood, flooding);

    to_flood = flooding;
}
