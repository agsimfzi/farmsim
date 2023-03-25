#pragma once

#include <SFML/System/Vector2.hpp>

#include <string>

/// DIRECTION ///
///
/// \brief an enumerated type for storing cardinal and intercardinal directions
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

/// directionToString ///
///
/// \brief converts passed direction to a string and returns it
///
std::string directionToString(Direction d);

/// invertDirection ///
///
/// \brief Returns the opposite direction.
///
Direction invertDirection(const Direction direction);

/// mirrorDirection ///
///
/// \brief returns the direction mirrored along the N-S axis
///
Direction mirrorDirection(Direction d);

/// randomDirection ///
///
/// \brief Returns a random direction.
///
Direction randomDirection();

/// randomOrthogonal ///
///
/// \brief returns a random orthogonal (cardinal) direction
///
Direction randomOrthogonal();

/// randomDiagonal ///
///
/// \brief returns a random diagonal (intercardinal) direction
///
Direction randomDiagonal();

/// randomPerpendicularDirection
///
/// \brief Returns a random direction along the orthogonal axis of the passed direction
///
Direction randomPerpendicularDirection(Direction odirect);

/// moveDirection ///
///
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

/// normalizeDirection ///
///
/// \brief Returns a direction relative to a given reference frame.
///
Direction normalizeDirection(const Direction ref, const Direction d);

/// isOrthogonal ///
///
/// \brief returns true if the direction is cardinal
///
bool isOrthogonal(Direction d);

/// isDiagonal ///
///
/// \brief returns true if the direction is intercardinal
///
bool isDiagonal(Direction d);

/// directionFromVector ///
///
/// \brief returns the best-match direction for the passed vector
///
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
