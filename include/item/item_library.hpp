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

    Item* operator ()(size_t uid) { return uidShelf[uid].get(); }
    Item* operator ()(std::string name) { return item(name); }

    std::shared_ptr<Item> shared(size_t uid);
    std::shared_ptr<Item> shared(std::string name);

private:
    std::map<std::string, std::shared_ptr<Item>> stringShelf;
    std::map<size_t, std::shared_ptr<Item>> uidShelf;
};
