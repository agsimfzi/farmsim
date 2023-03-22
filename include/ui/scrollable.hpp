#pragma once

#include <SFML/Graphics.hpp>

/// SCROLLABLE ///
///
/// \brief base class for scrollable UI elements
class Scrollable {
public:

/// DEFAULT CONSTRUCTOR ///
///
/// \brief the default constructor is insufficient. setView() and setScrollable() BOTH MUST BE CALLED
///
    Scrollable() = default;

/// setView ///
///
/// \brief defines the scrollable's viewport
///
/// \param pos The viewport's position (top-left origin)
/// \param size The viewport's size
/// \param window_size For properly applying the other values to the viewport
///
    void setView(sf::Vector2f pos, sf::Vector2f size, sf::Vector2u window_size = sf::Vector2u(1920u, 1080u));

/// getView ///
///
/// \brief returns the view
///
    sf::View getView();

/// reset ///
///
/// \brief minimizes the viewport and scrolls to top
    void reset();

/// setScrollable ///
///
/// \brief preps the frame and scrollbar
///
/// \param max_height Height of the full contents
///
    void setScrollable(float max_height);

/// scroll ///
///
/// \brief scrolls the contents if appropriate
///
    bool scroll(float delta, sf::Vector2f mpos);

/// scrollToTop ///
///
/// \brief sets the scroll height to 0
///
    void scrollToTop();

/// resizeScrollbar ///
///
/// \brief calculates scrollbar size based on viewport height against max height
///
    void resizeScrollbar();

/// placeScrollbar ///
///
/// \brief places the scrollbar based on the current scroll against the max scroll
///
    void placeScrollbar();

/// contains ///
///
/// \brief returns true if the passed mouse pos is inside the frame. pos must be calculated with the view outside of the class.
    bool contains(sf::Vector2f mpos);

protected:
    sf::RectangleShape scrollbar;

    sf::FloatRect frame;

    sf::Vector2f size; /**< viewport size */

    sf::View view; /**< viewport */

    float scroll_x_pos; /**< x-position of the scrollbar. on the right edge by default */

    float current_scroll = 0.f; /**< current scroll position */

    float max_scroll; /**< max scroll position (max height - viewport height) */
};
