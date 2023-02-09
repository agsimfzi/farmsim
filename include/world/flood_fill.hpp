#pragma once

#include <SFML/System/Vector2.hpp>

#include <queue>

#include "tile.hpp"

#include <iostream>

inline void floodFill(int x, int y, Map_Tile<bool>& to_flood, Map_Tile<bool>& flooding)
{
    // BE *VERY* CAREFUL ABOUT USING THIS RECURSIVE FLOOD FILL
    // IT ***WILL*** CAUSE A STACK OVERFLOW!!!!!!!!!
    // RECURSION IS ACCEPTABLE FOR DRAINING COASTAL LAKES BECAUSE THEY DON'T GET BIG...
        // (BUT EVEN THEN YOU SHOULD PROBABLY SWITCH OFF IT)
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

    int x = flooding.begin()->first;
    int y = flooding.begin()->second.begin()->first;

    int nx = flooding.rbegin()->first;
    int ny = flooding.rbegin()->second.rbegin()->first;

    for (int ix = x; ix <= nx; ix++) {
        for (int iy = y; iy <= ny; iy++) {
            flooding[ix][iy] = false;
        }
    }

    std::queue<sf::Vector2i> q;

    q.push(sf::Vector2i(x, y));

    while(q.size() > 0) {
        sf::Vector2i n = q.front();
        q.pop();
        if (!flooding[n.x][n.y]) {
            flooding[n.x][n.y] = true;
            if (to_flood[n.x - 1][n.y] && !flooding[n.x - 1][n.y]) {
                q.push(sf::Vector2i(n.x - 1, n.y));
            }
            if (to_flood[n.x + 1][n.y] && !flooding[n.x + 1][n.y]) {
                q.push(sf::Vector2i(n.x + 1, n.y));
            }
            if (to_flood[n.x][n.y - 1] && !flooding[n.x][n.y - 1]) {
                q.push(sf::Vector2i(n.x, n.y - 1));
            }
            if (to_flood[n.x][n.y + 1] && !flooding[n.x][n.y + 1]) {
                q.push(sf::Vector2i(n.x, n.y + 1));
            }
        }
    }

/*
  1. Set Q to the empty queue or stack.
  2. Add node to the end of Q.
  3. While Q is not empty:
  4.   Set n equal to the first element of Q.
  5.   Remove first element from Q.
  6.   If n is Inside:
         Set the n
         Add the node to the west of n to the end of Q.
         Add the node to the east of n to the end of Q.
         Add the node to the north of n to the end of Q.
         Add the node to the south of n to the end of Q.
  7. Continue looping until Q is exhausted.
  8. Return.

    floodFill(x, y, to_flood, flooding);
*/
    to_flood = flooding;
}
/*
inline void drainFill(int x, int y, Map_Tile<bool>& to_drain)
{
    to_drain[x][y] = false;

    if (to_drain[x - 1][y]) {
        drainFill(x - 1, y, to_drain);
    }
    if (to_drain[x + 1][y]) {
        drainFill(x + 1, y, to_drain);
    }
    if (to_drain[x][y - 1]) {
        drainFill(x, y - 1, to_drain);
    }
    if (to_drain[x][y + 1]) {
        drainFill(x, y + 1, to_drain);
    }
}
*/
inline void drainFill(int x, int y, Map_Tile<bool>& draining)
{
    std::queue<sf::Vector2i> q;

    q.push(sf::Vector2i(x, y));

    while(q.size() > 0) {
        sf::Vector2i n = q.front();
        q.pop();
        if (!draining[n.x][n.y]) {
            draining[n.x][n.y] = true;
            if (!draining[n.x - 1][n.y]) {
                q.push(sf::Vector2i(n.x - 1, n.y));
            }
            if (!draining[n.x + 1][n.y]) {
                q.push(sf::Vector2i(n.x + 1, n.y));
            }
            if (!draining[n.x][n.y - 1]) {
                q.push(sf::Vector2i(n.x, n.y - 1));
            }
            if (!draining[n.x][n.y + 1]) {
                q.push(sf::Vector2i(n.x, n.y + 1));
            }
        }
    }

/*
  1. Set Q to the empty queue or stack.
  2. Add node to the end of Q.
  3. While Q is not empty:
  4.   Set n equal to the first element of Q.
  5.   Remove first element from Q.
  6.   If n is Inside:
         Set the n
         Add the node to the west of n to the end of Q.
         Add the node to the east of n to the end of Q.
         Add the node to the north of n to the end of Q.
         Add the node to the south of n to the end of Q.
  7. Continue looping until Q is exhausted.
  8. Return.

    floodFill(x, y, to_flood, flooding);
*/
std::cout << "\tdrained!\n";
}
