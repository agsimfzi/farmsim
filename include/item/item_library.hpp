#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "item.hpp"

class Item_Library {
public:
    Item_Library();

    Item* item(std::string name);
    Item* item(size_t uid);

private:
    std::map<std::string, std::shared_ptr<Item>> stringShelf;
    std::map<size_t, std::shared_ptr<Item>> uidShelf;
};
