#include <world/direction.hpp>

#include <util/prng.hpp>

//////////////////////////////////////////////////////////////

std::string directionToString(Direction d)
{
    switch (d) {
        case Direction::N:
            return "NORTH";
        case Direction::NE:
            return "NORTHEAST";
        case Direction::E:
            return "EAST";
        case Direction::SE:
            return "SOUTHEAST";
        case Direction::S:
            return "SOUTH";
        case Direction::SW:
            return "SOUTHWEST";
        case Direction::W:
            return "WEST";
        case Direction::NW:
            return "NORTHWEST";
        default:
            return "NULLDIRECTION";
    }
}

/////////////////////////////////////////////////////////////
Direction invertDirection(const Direction d)
{
    switch (d) {
        case Direction::N:
            return Direction::S;
        case Direction::NE:
            return Direction::SW;
        case Direction::E:
            return Direction::W;
        case Direction::SE:
            return Direction::NW;
        case Direction::S:
            return Direction::N;
        case Direction::SW:
            return Direction::NE;
        case Direction::W:
            return Direction::E;
        case Direction::NW:
            return Direction::SE;
        default:
            return d;
    }
}

Direction mirrorDirection(Direction d)
{
    switch (d) {
        case Direction::NE:
            return Direction::NW;
        case Direction::E:
            return Direction::W;
        case Direction::SE:
            return Direction::SW;
        case Direction::SW:
            return Direction::SE;
        case Direction::W:
            return Direction::E;
        case Direction::NW:
            return Direction::NE;
        default:
            return d;
    }
}

/////////////////////////////////////////////////////////////
Direction randomDirection()
{
    return static_cast<Direction>(prng::number(0, static_cast<int>(Direction::NW)));
}

Direction randomOrthogonal()
{
    return static_cast<Direction>(2 * prng::number(0, 3));
}

Direction randomDiagonal()
{
    return static_cast<Direction>((2 * prng::number(0, 3)) + 1);
}

//////////////////////////////////////////////////////////////

Direction randomPerpendicularDirection(Direction odirect)
{
    switch (odirect) {
        case Direction::N:
        case Direction::S:
            odirect = Direction::W;
            break;
        case Direction::E:
        case Direction::W:
            odirect = Direction::N;
            break;
        case Direction::NE:
        case Direction::SW:
            odirect = Direction::NW;
            break;
        case Direction::NW:
        case Direction::SE:
            odirect = Direction::NE;
            break;
        default:
            odirect = Direction::NULL_DIRECTION;
            break;
    }

    if (odirect != Direction::NULL_DIRECTION
        && prng::boolean()) {
        odirect = invertDirection(odirect);
    }

    return odirect;
}

/////////////////////////////////////////////////////////////

Direction normalizeDirection(const Direction ref, const Direction d)
{
    Direction r;
    switch (ref) {
        case Direction::W:
            if (d == Direction::W) {
                r = Direction::S;
            }
            else if (d == Direction::E) {
                r = Direction::N;
            }
            else if (d == Direction::N) {
                r = Direction::W;
            }
            else if (d == Direction::S) {
                r = Direction::E;
            }
            break;
        case Direction::E:
            if (d == Direction::W) {
                r = Direction::N;
            }
            else if (d == Direction::E) {
                r = Direction::S;
            }
            else if (d == Direction::N) {
                r = Direction::E;
            }
            else if (d == Direction::S) {
                r = Direction::W;
            }
            break;
        case Direction::N:
            r = d;
            break;
        case Direction::S:
            if (d == Direction::W) {
                r = Direction::E;
            }
            else if (d == Direction::E) {
                r = Direction::W;
            }
            else if (d == Direction::N) {
                r = Direction::S;
            }
            else if (d == Direction::S) {
                r = Direction::N;
            }
            break;
        default:
            r = Direction::NULL_DIRECTION;
            break;
    } //case NORTH is excluded because Direction is already normalized to it

    return r;
}

bool isOrthogonal(Direction d)
{
    return (d == Direction::N
        || d == Direction::W
        || d == Direction::E
        || d == Direction::S);
}

bool isDiagonal(Direction d)
{
    return (d == Direction::NW
        || d == Direction::NE
        || d == Direction::SW
        || d == Direction::SE);
}
