#include <world/machine.hpp>

#include <util/primordial.hpp> // for equalStrings
#include <util/prng.hpp>

Machine::Machine()
{
    interface = true;
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
    // REAGANT ROW SIZE IS SET IN ::countReagants()!
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
        inventory.back().resize(1);
}

bool Machine::validReagant(std::string name, int rxn)
{
    if (rxn == -1) {
        for (const auto& r : reactions) {
            for (const auto& reagant : r.reagants) {
                if (equalStrings(name, reagant.name)) {
                    return true;
                }
            }
        }
    }
    else {
        for (const auto& reagant : reactions[rxn].reagants) {
            if (equalStrings(name, reagant.name)) {
                return true;
            }
        }
    }
    return false;
}

void Machine::checkReaction()
{
    int n = reactions.size();
    Item* product = activeProduct().get();
    for (int i = 0; i < n; i++) {
        if (checkReagants(i)
        && (!product || equalStrings(product->getName(), reactions[i].product))) {
            if (i != current_reaction) {
                reaction_tick = 0;
                current_reaction = i;
            }
            return;
        }
    }
    endReaction();
}

bool Machine::checkReagants(size_t i)
{
    size_t n = 1;
    Item* product = activeProduct().get();
    if ((int)i >= 0 && (!product || (int)product->count() < product->stackSize())) {
        std::vector<Reagant> reagants = reactions[i].reagants;
        for (const auto& item : inventory.front()) {
            if (item) {
                auto it = std::find_if(reagants.begin(), reagants.end(), [&](Reagant& r) { return equalStrings(item->getName(), r.name); });
                if (it == reagants.end()) {
                    return false;
                }
                else {
                    reagants.erase(it);
                }
            }
        }
        n = reagants.size();
    }
    return (n == 0);
}

void Machine::tick(Item_Library& item_library)
{
    if (current_reaction >= 0) {
        Item* product = activeProduct().get();
        if (!product
        || equalStrings(product->getName(), reactions[current_reaction].product)) {
            size_t count = 1;
            if (machine_type == Machine_Type::SEED_EXTRACTOR) {
                count += prng::boolean(0.5);
                count += prng::boolean(0.25);
            }
            reaction_tick++;
            if (reaction_tick >= reactions[current_reaction].length) {
                if (!product) {
                    inventory.back().back() = std::make_shared<Item>(*item_library(reactions[current_reaction].product));
                }
                else {
                    product->setCount(product->count() + count);
                }

                for (auto& active_reagant : inventory.front()) {
                    if (active_reagant && validReagant(active_reagant->getName(), current_reaction)) {
                        active_reagant->setCount(active_reagant->count() - 1);
                        if (active_reagant->count() == 0) {
                            active_reagant = nullptr;
                            endReaction();
                        }
                    }
                }

                reaction_tick = 0;
            }
        }
    }
}

float Machine::reactionProgress()
{
    if (current_reaction < 0) {
        return 0.f;
    }
    return (static_cast<float>(reaction_tick) / static_cast<float>(reactions[current_reaction].length));
}

void Machine::endReaction()
{
    current_reaction = -1;
    reaction_tick = 0;
}

void Machine::clearReagant(size_t i)
{
    inventory.front()[i].reset();
    checkReaction();
}

void Machine::clearProduct()
{
    inventory.back().back().reset();
    checkReaction();
}

std::vector<std::shared_ptr<Item>> Machine::activeReagants()
{
    return inventory.front();
}

std::shared_ptr<Item>& Machine::activeProduct()
{
    return inventory.back().back();
}

size_t Machine::addReagant(std::shared_ptr<Item> item)
{
    size_t remainder = item->count();
    if (item && validReagant(item->getName(), current_reaction)) {
        int first_empty_index = -1;
        size_t n = inventory.front().size();
        for (size_t i = 0; i < n; i++) {
            if (inventory.front()[i]) {
                if (inventory.front()[i]->getUID() == item->getUID()) {
                    inventory.front()[i]->add(item->count());
                    remainder = 0;
                    break;
                }
            }
            else if (first_empty_index < 0) {
                first_empty_index = i;
            }
        }
        if (first_empty_index >= 0) {
            inventory.front()[first_empty_index] = std::make_shared<Item>(*item);
            remainder = 0;
        }
    }
    checkReaction();
    return remainder;
}

void Machine::setReagant(std::shared_ptr<Item> item, size_t i)
{
    inventory.front()[i] = item;
    checkReaction();
}

void Machine::setProduct(std::shared_ptr<Item> item)
{
    inventory.back().back() = item;
    checkReaction();
}

void Machine::countReagants()
{
    for (const auto& r : reactions) {
        const size_t n = r.reagants.size();
        if (max_reagant_count < n) {
            max_reagant_count = n;
        }
    }
    inventory[inventory.size() - 2].resize(max_reagant_count);

}
