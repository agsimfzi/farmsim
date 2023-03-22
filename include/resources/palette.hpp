#pragma once

#include <SFML/Graphics/Color.hpp>

/// PALETTE ///
///
/// \brief stores colors for visual consistency
///
namespace Palette {
    // NEUTRAL
    const sf::Color black = sf::Color(10, 10, 10);      /**< darkest neutral */
    const sf::Color gray_dark = sf::Color(25, 25, 25);  /**< darker neutral */
    const sf::Color gray = sf::Color(90, 90, 90);       /**<mid-dark neutral */
    const sf::Color white = sf::Color(245, 245, 245);   /**< lightest neutral */

    // COLORS
    const sf::Color green = sf::Color(75, 125, 64);
    const sf::Color blue = sf::Color(95, 75, 145);
    const sf::Color purple = sf::Color(36, 10, 51);
    const sf::Color orange = sf::Color(241, 126, 30);

    // INVENTORY
    const sf::Color inventory_bg = sf::Color(239, 209, 154);            /**< for regular inventory cells and associated UI elements */
    const sf::Color inventory_bg_active = sf::Color(252, 244, 212);     /**< for active (equipped) inventory cells*/
    const sf::Color inventory_bg_unavailable = sf::Color(82, 82, 82);   /**< for unavailable reaction panels */
    const sf::Color inventory_outline = sf::Color(25, 25, 25);          /**< for inventory frames */
}
