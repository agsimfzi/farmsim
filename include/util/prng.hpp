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

#include <random>
#include <type_traits>

/////////////////////////////////////////////////////////////
/// \brief Pseudo-random number generation (and bools, and chars)
///
namespace prng {

/////////////////////////////////////////////////////////////
/// \brief Internal wrapper struct to seed the rng.
///
struct Engine : public std::mt19937 {
    Engine()
        : std::mt19937(std::random_device()())
    {
    }
};

/////////////////////////////////////////////////////////////
/// \brief Internal function which returns the rng engine.
///
inline std::mt19937& engine()
{
    static Engine mt;
    return mt;
}

/////////////////////////////////////////////////////////////
/// \brief Returns a random floating-point number (float or double)
///
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
number(const T floor, const T ceil)
{
    std::uniform_real_distribution<T> dist(floor, ceil);
    return dist(engine());
}

/////////////////////////////////////////////////////////////
/// \brief Returns a random integral type (int, char)
///
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
number(const T floor, const T ceil)
{
    std::uniform_int_distribution<T> dist(floor, ceil);
    return dist(engine());
}

/////////////////////////////////////////////////////////////
/// \brief Returns a random container accessor based on its size.
///
template <typename size_type>
number(size_type s)
{
    return number(0u, (unsigned int)s - 1);
}

/////////////////////////////////////////////////////////////
/// \brief Returns a random bool (50% chance)
///
inline bool boolean()
{
    std::bernoulli_distribution dist;
    return dist(engine());
}

/////////////////////////////////////////////////////////////
/// \brief Returns a weighted bool (percent chance out of 1.f)
///
inline bool boolean(float chance)
{
    std::bernoulli_distribution dist(chance);
    return dist(engine());
}
}
