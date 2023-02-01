#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>

/////////////////////////////////////////////////////////////
/// \brief
///
enum class Direction {
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW,
    NULL_DIRECTION
};

std::string directionToString(Direction d);

/////////////////////////////////////////////////////////////
/// \brief Returns the opposite direction.
///
Direction invertDirection(const Direction direction);

Direction mirrorDirection(Direction d);

/////////////////////////////////////////////////////////////
/// \brief Returns a random direction.
///
Direction randomDirection();

Direction randomOrthogonal();

Direction randomDiagonal();

/////////////////////////////////////////////////////////////
/// \brief Returns a random direction perpendicular to odirect.
///
Direction randomPerpendicularDirection(Direction odirect);

/////////////////////////////////////////////////////////////
/// \brief Increments a vector depending on direction.
///
template <typename T>
void moveDirection(sf::Vector2<T>& v, const Direction direction, const T distance)
{
    switch (direction) {
        case Direction::N:
            v.y -= distance;
            break;
        case Direction::S:
            v.y += distance;
            break;
        case Direction::E:
            v.x += distance;
            break;
        case Direction::W:
            v.x -= distance;
            break;
        default:
            break;
    }
}

/////////////////////////////////////////////////////////////
/// \brief Returns a direction relative to a given reference frame.
///
Direction normalizeDirection(const Direction ref, const Direction d);

bool isOrthogonal(Direction d);

bool isDiagonal(Direction d);

inline Direction directionFromVector(sf::Vector2i v)
{
    Direction d = Direction::NULL_DIRECTION;
    bool xless = (v.x < 0);
    bool xmore = (v.x > 0);
    bool yless = (v.y < 0);
    bool ymore = (v.y > 0);

    if (xless) {
        if (yless) {
            d = Direction::NW;
        }
        else if (ymore) {
            d = Direction::SW;
        }
        else {
            d = Direction::W;
        }
    }
    else if (xmore) {
        if (yless) {
            d = Direction::NE;
        }
        else if (ymore) {
            d = Direction::SE;
        }
        else {
            d = Direction::E;
        }
    }
    else if (yless) {
        d = Direction::N;
    }
    else if (ymore) {
        d = Direction::S;
    }

    return d;
}
