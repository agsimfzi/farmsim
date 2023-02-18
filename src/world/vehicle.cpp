#include <world/vehicle.hpp>

#include <resources/texture_manager.hpp>

Vehicle::Vehicle(Vehicle::Type type, sf::Vector2f pos)
    : type{ type }
{
    setPosition(pos);
    setTexture(Texture_Manager::get("VEHICLE0"));
    setOrigin(sf::Vector2f(getGlobalBounds().width / 2.f, getGlobalBounds().height / 2.f));
}
