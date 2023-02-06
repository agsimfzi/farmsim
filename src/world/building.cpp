#include <world/building.hpp>

#include <util/primordial.hpp>

#include <iostream>

bool Building::validReagant(std::string name)
{
    //std::cout << "testing " << name << " as valid reagant against...\n";
    for (const auto& r : reactions) {
        //std::cout << "\t" << r.reagant << "...";
        if (equalStrings(name, r.reagant)) {
            //std::cout << "TRUE!\n";
            return true;
        }
        //std::cout << '\n';
    }

    return false;
}

void Building::checkReaction()
{
    if (active_reagant) {
        size_t n = reactions.size();
        int rxn = -1;
        for (size_t i = 0; i < n; i++) {
            if (equalStrings(active_reagant->getName(), reactions[i].reagant)) {
                rxn = i;
                break;
            }
        }
        if (rxn != current_reaction) {
            current_reaction = rxn;
            reaction_tick = 0;
        }
    }
}

void Building::tick(Item_Library& item_library)
{
    //std::cout << "running building tick...\n";
    if (current_reaction >= 0) {
        if (!active_product
        || equalStrings(active_product->getName(), reactions[current_reaction].product)) {
            reaction_tick++;
            if (reaction_tick >= reactions[current_reaction].length) {
                if (!active_product) {
                    active_product = std::make_shared<Item>(*item_library(reactions[current_reaction].product));
                    active_product->setCount(1);
                }
                else {
                    active_product->setCount(active_product->count() + 1);
                }

                active_reagant->setCount(active_reagant->count() - 1);
                if (active_reagant->count() == 0) {
                    active_reagant = nullptr;
                    current_reaction = -1;
                }

                reaction_tick = 0;
            }
        }
    }
}

float Building::progress()
{
    return (static_cast<float>(reaction_tick) / static_cast<float>(reactions[current_reaction].length));
}

void Building::endReaction()
{
    current_reaction = -1;
    reaction_tick = 0;
}

void Building::clearReagant()
{
    active_reagant.reset();
    endReaction();
}

void Building::clearProduct()
{
    active_product.reset();
}

Item* Building::activeReagant()
{
    return active_reagant.get();
}

Item* Building::activeProduct()
{
    return active_product.get();
}

void Building::setReagant(Item* item)
{
    if (item) {
        if (active_reagant) {
            active_reagant->add(item->count());
        }
        else {
            active_reagant = std::make_shared<Item>(*item);
            checkReaction();
        }
    }
    else if (active_reagant) {
        active_reagant.reset();
        endReaction();
    }
}

void Building::setProduct(Item* item)
{
    if (item) {
        if (active_product) {
            active_product->add(item->count());
        }
        else {
            active_product = std::make_shared<Item>(*item);
        }
    }
    else if (active_product) {
        active_product.reset();
        endReaction();
    }
}
