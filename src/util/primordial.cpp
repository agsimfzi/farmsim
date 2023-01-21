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

#include <util/primordial.hpp>
#include <util/prng.hpp>

//////////////////////////////////////////////////////////////

sf::Color randomColor(unsigned int alpha)
{
    unsigned int r = prng::number(0, 255);
    unsigned int g = prng::number(0, 255);
    unsigned int b = prng::number(0, 255);
    return sf::Color(r, g, b, alpha);
}

sf::Color randomColor()
{
    return randomColor(255);
}

void centerText(sf::Text& text)
{
    sf::FloatRect trect = text.getLocalBounds();
    text.setOrigin(trect.left + trect.width / 2.f, trect.top + trect.height / 2.f);
}

sf::FloatRect getViewBounds(const sf::View& view)
{
    return sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
}

float calculateAngle(sf::Vector2f pos1, sf::Vector2f pos2)
{
    float theta;

    //defines a right triangle between two points
    float opp = pos2.y - pos1.y;
    float adj = pos2.x - pos1.x;

    //calculates theta in radians
    theta = atan(opp / adj);

    //converts theta to degrees
    theta *= 180 / PI;

    //converts theta for use with SFML
    if (adj > 0) {
        theta += 90;
    }
    else if (adj < 0) {
        theta += 270;
    }

    return theta;
}

sf::Vector2f calculateMoveVector(sf::Vector2f start, sf::Vector2f end, float speed)
{
    sf::Vector2f v(0.f, 0.f);
    calculateMoveVector(calculateAngle(start, end), speed, v);
    return v;
}

void calculateMoveVector(float angle, float velocity, sf::Vector2f& moveVector)
{
    angle -= 90;
    angle *= (PI / 180); //converts angle to radians

    moveVector.x = velocity * cos(angle);
    moveVector.y = velocity * sin(angle);
}

int roundFloat(float f)
{
    return static_cast<int>(f + 0.5f);
}
