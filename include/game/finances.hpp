#pragma once

#include <map>

#include <item/item_type.hpp>

#include <world/season.hpp>

struct Financial_Record {
    std::map<Item_Type, std::map<size_t, long long int>> gains; /**< amount earned by selling items (keyed to UID) */
    std::map<Item_Type, std::map<size_t, long long int>> losses; /**< amount spent on items (keyed to UID) */
};

class Player_Finances {
public:
    Player_Finances() = default;

    void addGain(unsigned int year, Season season, size_t uid, Item_Type type, int amount);
    void addLoss(unsigned int year, Season season, size_t uid, Item_Type type, int amount);

private:
    std::map<unsigned int, std::map<Season, Financial_Record>> seasonal;
    Financial_Record total;
};
