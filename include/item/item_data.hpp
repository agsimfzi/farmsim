#pragma once

#include "item_type.hpp"

class Item_Data {
public:
    size_t uid;
    std::string name;
    Item_Type type;
    std::string subtype;
    std::string description;
    int value;
    int stack_size;
};
