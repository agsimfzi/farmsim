////////////////////////////////////////////////////////////
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

#include <SFML/System/Vector2.hpp>

#include <math.h>

#define PI 3.14159265359

/////////////////////////////////////////////////////////////
/// \brief
///
sf::Color randomColor(unsigned int alpha);

/////////////////////////////////////////////////////////////
/// \brief
///
sf::Color randomColor();

template <typename T>
sf::Vector2<T> normalizeVector(sf::Vector2<T> v)
{
    T x = 0;
    if (v.x > 0) {
        x = 1;
    }
    else if (v.x < 0) {
        x = -1;
    }

    T y = 0;
    if (v.y > 0) {
        y = 1;
    }
    else if (v.y < 0) {
        y = -1;
    }

    return sf::Vector2<T>(x, y);
}

/////////////////////////////////////////////////////////////
/// \brief
///
void centerText(sf::Text& text);

sf::FloatRect getViewBounds(const sf::View& view);

/////////////////////////////////////////////////////////////
/// \brief Returns the scalar distance between two 2d vectors.
///
template <typename T>
T scalarDistance(const sf::Vector2<T> v, sf::Vector2<T> const w)
{
    T a = std::abs(w.x - v.x);
    T b = std::abs(w.y - v.y);
    return static_cast<T>(sqrt(pow(a, 2) + pow(b, 2)));
}

template <typename T>
T scalarDistance(const sf::Vector2<T> v)
{
    T a = std::abs(v.x);
    T b = std::abs(v.y);
    return static_cast<T>(sqrt(pow(a, 2) + pow(b, 2)));
}

/////////////////////////////////////////////////////////////
/// \brief Returns the 2d vector distance between two 2d vectors.
///
template <typename T>
sf::Vector2<T> vectorDistance(const sf::Vector2<T> v, const sf::Vector2<T> w)
{
    return sf::Vector2<T>(w.x - v.x, w.y - v.y);
}

template <typename T>
double calculateOrientation(sf::Vector2<T> v)
{
    double t = atan((double)v.y / (double)v.x);

    t *= 180.d / PI;

    if (v.x >= 0)
        t += 90;
    else if (v.x < 0)
        t += 270;

    return t;
}

template <typename T>
double calculateOrientation(sf::Vector2<T> v, sf::Vector2<T> w)
{
    return calculateOrientation(w - v);
}

////////////////////////////////////////////////////////////
/// \brief Calculates the angle needed to point an object towards
/// another object or point.
///
/// \param \b pos1
/// \param \b pos2
///
/// \return \b theta in degrees
///
/// \see calculateDistance(), calculateMovementVector(), calculateVelocity()
///
float calculateAngle(sf::Vector2f pos1, sf::Vector2f pos2);

////////////////////////////////////////////////////////////
/// \brief Defines a movement vector based on speed and angle.
///
/// \param \b angle in degrees
/// \param \b velocity
/// \param \b moveVector a reference to an object's movement vector
///
/// \see calculateOrientation(), calculateDistance(), calculateVelocity()
///
void calculateMoveVector(float angle, float velocity, sf::Vector2f& moveVector);
sf::Vector2f calculateMoveVector(sf::Vector2f start, sf::Vector2f end, float speed);

int roundFloat(float f);

std::string makeUppercase(std::string str);
bool equalStrings(std::string s1, std::string s2);

template <typename T>
T scalarProduct(const sf::Vector2<T>& v, const sf::Vector2<T>& w)
{
    return ((v.x * w.x) + (v.y * w.y));
}

template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}
