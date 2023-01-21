#pragma once

#include <SFML/System/Vector2.hpp>

#include <iostream>
#include <string>

/////////////////////////////////////////////////////////////
/// \brief
///
template <typename T>
std::ostream& operator<<(std::ostream& os, sf::Vector2<T> v)
{
    os << v.x << ", " << v.y;
    return os;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, sf::Rect<T> r)
{
    os << r.left << ", " << r.top << "; " << r.width << ", " << r.height;
    return os;
}
