#pragma once

#include <SFML/Graphics/Text.hpp>

#include <SFML/System/Vector2.hpp>

bool charInBounds(sf::Text& text, unsigned int i, int width);

////////////////////////////////////////////////////////////
/// \brief Inserts newlines into a text object's string so it fits its container.
///
/// \param \b text the text to be wrapped
/// \param \b width the width of the text's container
///
void wrapText(sf::Text& text, int width);
