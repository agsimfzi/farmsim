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

/// randomColor ///
///
/// \brief returns a random color
///
/// \param alpha Transparency
///
sf::Color randomColor(unsigned int alpha);

/// randomColor ///
///
/// \brief returns a random color of full transparency
///
sf::Color randomColor();

/// normalizeVector ///
///
/// \brief creates a unit vector (all values -1, 0, or 1)
///
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

/// centerText ///
///
/// \brief modifies the passed text object to have a centered origin
///
void centerText(sf::Text& text);

/// getViewBounds ///
///
/// \brief returns a floatrect of the passed viewport's global bounds
sf::FloatRect getViewBounds(const sf::View& view);

/// scalarDistance ///
///
/// \brief Returns the scalar distance between two vector2's
///
template <typename T>
T scalarDistance(const sf::Vector2<T> v, sf::Vector2<T> const w)
{
    T a = std::abs(w.x - v.x);
    T b = std::abs(w.y - v.y);
    return static_cast<T>(sqrt(pow(a, 2) + pow(b, 2)));
}

/// scalarDistance ///
///
/// \brief Returns the scalar distance between a vector2 and the world origin
///
template <typename T>
T scalarDistance(const sf::Vector2<T> v)
{
    T a = std::abs(v.x);
    T b = std::abs(v.y);
    return static_cast<T>(sqrt(pow(a, 2) + pow(b, 2)));
}

/// vectorDistance ///
///
/// \brief Returns the 2d vector distance between two 2d vectors.
///
template <typename T>
sf::Vector2<T> vectorDistance(const sf::Vector2<T> v, const sf::Vector2<T> w)
{
    return sf::Vector2<T>(w.x - v.x, w.y - v.y);
}

/// calculateOrientation ///
///
/// \brief finds the angle, in degrees, between the passed vector2 and the world origin
///
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

/// calculateOrientation ///
///
/// \brief finds the angle, in degrees, between two passed vector2's
///
template <typename T>
double calculateOrientation(sf::Vector2<T> v, sf::Vector2<T> w)
{
    return calculateOrientation(w - v);
}

/// calculateAngle ///
///
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

/// calculateMoveVector ///
///
/// \brief Defines a movement vector based on speed and angle.
///
/// \param \b angle in degrees
/// \param \b velocity
/// \param \b moveVector a reference to an object's movement vector
///
/// \see calculateOrientation(), calculateDistance(), calculateVelocity()
///
void calculateMoveVector(float angle, float velocity, sf::Vector2f& moveVector);

/// calculateMoveVector ///
///
/// \brief returns a movement vector based on start pos, end pos, and speed
///
/// \param \b start vector2
/// \param \b end target vector2
/// \param \b speed max orthogonal velocity
///
/// \see calculateOrientation(), calculateDistance(), calculateVelocity()
///
sf::Vector2f calculateMoveVector(sf::Vector2f start, sf::Vector2f end, float speed);

/// roundFloat ///
///
/// \brief converts a float to an int, rounding it up
///
int roundFloat(float f);

/// makeUppercase ///
///
/// \brief transforms a string to all uppercase
///
std::string makeUppercase(std::string str);

/// equalStrings ///
///
/// \brief case-insensitive string comparison
///
bool equalStrings(std::string s1, std::string s2);

/// scalarProduct ///
///
/// \brief returns the dot product of two vector2's
///
template <typename T>
T scalarProduct(const sf::Vector2<T>& v, const sf::Vector2<T>& w)
{
    return ((v.x * w.x) + (v.y * w.y));
}

/// sign ///
///
/// \brief returns -1 for negative numbers, +1 for positive numbers
template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}
