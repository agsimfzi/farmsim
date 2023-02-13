#include <world/machine.hpp>

#include <util/primordial.hpp>

#include <iostream>

Machine::Machine()
{
    interface = true;
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
        inventory.back().push_back(std::shared_ptr<Item>());
        inventory.back().push_back(std::shared_ptr<Item>());
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
        inventory.back().push_back(std::shared_ptr<Item>());
}

bool Machine::validReagant(std::string name)
{
    for (const auto& r : reactions) {
        for (const auto& reagant : r.reagants) {
            if (equalStrings(name, reagant)) {
                //std::cout << "TRUE!\n";
                return true;
            }
        }
    }

    return false;
}

void Machine::checkReaction()
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

bool Machine::checkReagants()
{
    bool good = false;
    if (current_reaction >= 0) {
        good = true;
        std::vector<std::string> reagants = reactions[current_reaction].reagants;
        for (const auto& reagant : reactions[current_reaction].reagants) {
            for (size_t i = 0; i <)
        }
    }
    return good;
}

void Machine::tick(Item_Library& item_library)
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

float Machine::reactionProgress()
{
    return (static_cast<float>(reaction_tick) / static_cast<float>(reactions[current_reaction].length));
}

void Machine::endReaction()
{
    current_reaction = -1;
    reaction_tick = 0;
}

void Machine::clearReagant()
{
    active_reagant.reset();
    endReaction();
}

void Machine::clearProduct()
{
    active_product.reset();
}

Item* Machine::activeReagants()
{
    return inventory.front();
}

Item* Machine::activeProduct()
{
    return inventory.back().front();
}

bool Machine::addReagant(Item* item)
{
    bool added = false;
    if (item) {
        int first_empty_index = -1;
        size_t n = inventory.front().size();
        for (size_t i = 0; i < n; i++) {
            if (item) {
                if (inventory.front()[i]->getUID(), item->getUID()) {
                    inventory.front()[i]->add(item->count());
                    item = nullptr;
                    added = true;
                    break;
                }
            }
            else if (first_empty_index < 0) {
                first_empty_index = i;
            }
        }
        if (first_empty_index > 0 && item) {
            inventory.front()[first_empty_index] = std::make_shared<Item>(*item);
            added = true;
        }
    }
    return added;
}

void Machine::setProduct(Item* item)
{
    if (item) {
        if (inventory.back().front()) {
            inventory.back().front()->add(item->count());
        }
        else {
            inventory.back().front() = std::make_shared<Item>(*item);
        }
    }
    else if (inventory.back().front()) {
        inventory.back().front().reset();
        endReaction();
    }
}
