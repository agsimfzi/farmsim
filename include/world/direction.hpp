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
