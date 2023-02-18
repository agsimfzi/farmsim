#include <world/vehicle.hpp>

#include <resources/texture_manager.hpp>

Vehicle::Vehicle(Vehicle::Type type, sf::Vector2f pos)
    : type{ type }
{
    setPosition(pos);
    setTexture(Texture_Manager::get("VEHICLE0"));
    sf::Vector2i tpos;
    tpos.x = static_cast<int>(type) * 64;
    tpos.y = 0;
    sf::Vector2i tsize(64, 64);
    setTextureRect(sf::IntRect(tpos, tsize));
    setOrigin(sf::Vector2f(getGlobalBounds().width / 2.f, getGlobalBounds().height / 2.f));
}
