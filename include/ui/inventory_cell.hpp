#pragma once

#include <memory>

#include <item/item.hpp>

/// INVENTORY CELL ///
///
/// \brief UI element for displaying a single slotted item
///
class Inventory_Cell : public sf::Drawable {
public:

/// FULL CONSTRUCTOR ///
///
/// \brief defines the cell's elements, including an item if applicable. does not set position.
///
/// \param item nullable
///
    Inventory_Cell(std::shared_ptr<Item> item = nullptr);

/// setPosition ///
///
/// \brief places the frame, number_text, and item sprite at the passed position
///
    void setPosition(sf::Vector2f pos);

    const static float size; /**< frame size */
    const static float frame_thickness; /**< frame thickness */

    const static sf::Color colorUseBar;/**< color of the use bar */

/// activate ///
///
/// \brief colors the cell's background color for equipped items
///
    void activate();


/// deactivate ///
///
/// \brief sets the cell's background color to default
///
    void deactivate();

/// setItem ///
///
/// \brief adds an item to a cell. if an item is already present, it is overwritten.
/// if a present item is the same as the added item, the counts are instead combined.
///
    void setItem(std::shared_ptr<Item> i);

/// clearItem ///
///
/// \brief removes the item from the cell
///
    void clearItem();

/// getItem ///
///
/// \brief returns the cell's item
///
    std::shared_ptr<Item> getItem();

/// add ///
///
/// \brief adds to the cell's item's count
///
    void add(size_t count);

/// take ///
///
/// \brief substracts from the cell's item's count
///
    void take(size_t count);

/// updateCount ///
///
/// \brief updates the count text
///
    void updateCount();
/// setCount ///
///
/// \brief sets the number of items in the cell
///
    void setCount(size_t count);

/// contains ///
///
/// \brief checks if the cell's frame contains the passed position
///
    template <typename T>
    bool contains(sf::Vector2<T> pos)
    {
        return frame.getGlobalBounds().contains(sf::Vector2f(pos));
    }

/// getPosition ///
///
/// \brief returns the position of the frame (centered)
///
    sf::Vector2f getPosition();

    const static unsigned int character_size = 8; /**< character size for count text */

private:
    sf::RectangleShape frame;
    std::shared_ptr<Item> item = nullptr; /**< cell's stored item */

    sf::Text count_text; /**< displays the number of items */

    sf::RectangleShape use_bar; /**< visual element showing how many more times an item (watering can) may be used */
    const static sf::Vector2f use_bar_size; /**< provides height and max width of use bar */

    bool usable = false; /**< tracks whether to draw the use bar - only really for watering cans */


/// calculateUseBarSize ///
///
/// \brief internal function that updates the size of the "use bar", which is really only for watering cans.
///
    void calculateUseBarSize(int percent);

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
