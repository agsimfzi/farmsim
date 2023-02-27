#pragma once

#include <SFML/Graphics/Color.hpp>

/////////////////////////////////////////////////////////////
/// \brief
///
namespace Palette {

// NEUTRAL
const sf::Color black = sf::Color(10, 10, 10);
const sf::Color gray_dark = sf::Color(25, 25, 25);
const sf::Color white = sf::Color(245, 245, 245);
const sf::Color gray = sf::Color(90, 90, 90);

// COLORS
const sf::Color green = sf::Color(75, 125, 64);
const sf::Color blue = sf::Color(95, 75, 145);
const sf::Color purple = sf::Color(36, 10, 51);
const sf::Color orange = sf::Color(241, 126, 30);

// INVENTORY
// const sf::Color inventory_bg = sf::Color(230, 209, 135);
const sf::Color inventory_bg = sf::Color(239, 209, 154);
const sf::Color inventory_bg_active = sf::Color(252, 244, 212);
const sf::Color inventory_bg_unavailable = sf::Color(82, 82, 82);

const sf::Color inventory_outline = sf::Color(25, 25, 25);
}
