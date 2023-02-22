#include <world/container.hpp>

Container::Container()
{
    interface = true;
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
        inventory.back().resize(8);
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
        inventory.back().resize(8);
    inventory.push_back(std::vector<std::shared_ptr<Item>>());
        inventory.back().resize(8);
}

std::shared_ptr<Item> Container::getItem(sf::Vector2i i)
{
    return inventory[i.x][i.y];
}

void Container::addItem(std::shared_ptr<Item>& item)
{
    if (item) {
        sf::Vector2i first_empty_index(-1, -1);
        item->can_pickup = true;
        for (size_t r = 0; r < inventory.size(); r++) {
            for (size_t c = 0; c < inventory[r].size(); c++) {
                if (inventory[r][c] && item->getUID() == inventory[r][c]->getUID()) {
                    int stack = item->count() + inventory[r][c]->count();
                    if (stack <= item->stackSize()) {
                        inventory[r][c]->add(item->count());
                        item = nullptr;
                        return;
                    }
                    else {
                        size_t diff = item->stackSize() - inventory[r][c]->count();
                        item->take(diff);
                        inventory[r][c]->add(diff);
                    }
                }
                else if (!inventory[r][c] && first_empty_index == sf::Vector2i(-1, -1)) {
                    first_empty_index = sf::Vector2i(r, c);
                }
            }
        }
        if (first_empty_index != sf::Vector2i(-1, -1) && item) {
            inventory[first_empty_index.x][first_empty_index.y] = item;
            item = nullptr;
        }
    }
}

void Container::setItem(std::shared_ptr<Item> item, sf::Vector2i i)
{
    inventory[i.x][i.y].reset();
    inventory[i.x][i.y] = item;
}

void Container::clearItem(sf::Vector2i i)
{
    inventory[i.x][i.y].reset();
}
