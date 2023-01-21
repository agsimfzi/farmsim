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
    Item* item(unsigned int uid);

private:
    std::map<std::string, std::shared_ptr<Item>> stringShelf;
    std::map<unsigned int, std::shared_ptr<Item>> uidShelf;
};
