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

void Container::setItem(std::shared_ptr<Item> item, sf::Vector2i i)
{
    inventory[i.x][i.y] = item;
}

void Container::clearItem(sf::Vector2i i)
{
    inventory[i.x][i.y].reset();
}
