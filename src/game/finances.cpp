#include <game/finances.hpp>

void Player_Finances::addGain(unsigned int year, Season season, size_t uid, Item_Type type,  int amount)
{
    total.gains[type][uid] += amount;
    seasonal[year][season].gains[type][uid] += amount;
}

void Player_Finances::addLoss(unsigned int year, Season season, size_t uid, Item_Type type,  int amount)
{
    total.losses[type][uid] += amount;
    seasonal[year][season].losses[type][uid] += amount;
}
