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
