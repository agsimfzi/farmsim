#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "item.hpp"

/// ITEM LIBRARY ///
///
/// \brief stores item prototypes
///
class Item_Library {
public:
/// DEFAULT CONSTRUCTOR ///
///
/// \brief retrieves item data from the database and initializes items
    Item_Library();

/// OPERATOR () (SIZE_T) ///
///
/// \brief retrieves a new shared item pointer of the passed unique ID
///
    std::shared_ptr<Item> operator ()(size_t uid)
    {
        return std::make_shared<Item>(*uid_shelf[uid]);
    }

/// OPERATOR () (STRING) ///
///
/// \brief returns a new shared item pointer of the passed name
///
    std::shared_ptr<Item> operator ()(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        return std::make_shared<Item>(*string_shelf[name]);
    }

private:
    std::map<std::string, std::shared_ptr<Item>> string_shelf; /**< stores items by name */
    std::map<size_t, std::shared_ptr<Item>> uid_shelf; /**< stores items by unique ID */
};
