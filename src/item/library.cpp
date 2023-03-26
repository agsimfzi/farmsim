#include <item/library.hpp>

std::shared_ptr<Item> Library::item(std::string name)
{
    return items(name);
}

std::shared_ptr<Item> Library::item(size_t uid)
{
    return items(uid);
}

std::shared_ptr<Building> Library::building(std::string name)
{
    return buildings(name);
}

std::shared_ptr<Building> Library::building(size_t uid)
{
    return buildings(uid);
}

Crop Library::crop(size_t uid)
{
    return crops(uid);
}

Vehicle_Data Library::vehicle(Vehicle::Type type)
{
    return vehicles(type);
}

Detail Library::detail(std::string name)
{
    return details(name);
}
