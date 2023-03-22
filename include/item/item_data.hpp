#pragma once

#include "item_type.hpp"

/// ITEM DATA ///
///
/// \brief stores data for a single Item object
///
class Item_Data {
public:
    size_t uid; /**< unique identifier */
    std::string name;
    Item_Type type;
    std::string subtype; /**< for various bits of logic */
    std::string description;
    int value; /**< base monetary value */
    int stack_size; /**< maximum number of items in one stack */
    int use_percent = 100; /**< for watering cans */
    int use_factor; /**< tracks the magnitude of an item's effect, for e.g. tools and edibles */
};
