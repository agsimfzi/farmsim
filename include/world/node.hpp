#pragma once

#include <SFML/System/Vector2.hpp>

/////////////////////////////////////////////////////////////
/// \brief World coordinate node.
///
struct Node {
    Node(sf::Vector2i ncoord)
        : coordinates { ncoord }
    {}
    sf::Vector2i coordinates;
};
