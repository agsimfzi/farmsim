#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "item.hpp"

class Item_Library {
public:
    Item_Library();

    std::shared_ptr<Item> operator ()(size_t uid)
    {
        return std::make_shared<Item>(*uid_shelf[uid]);
    }

    std::shared_ptr<Item> operator ()(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        return std::make_shared<Item>(*string_shelf[name]);
    }

private:
    std::map<std::string, std::shared_ptr<Item>> string_shelf;
    std::map<size_t, std::shared_ptr<Item>> uid_shelf;
};
