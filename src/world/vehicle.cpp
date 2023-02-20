#include <world/vehicle.hpp>

#include <resources/texture_manager.hpp>

Vehicle::Vehicle(sf::Vector2f pos, Vehicle_Data d)
    : Animated_Sprite<Vehicle_State>(Texture_Manager::get(d.name), d.size, d.counts, d.thresholds)
    , type{ stringToType(d.name) }
    , speed_factor{ d.speed_factor }
{
    setPosition(pos);
    /*
    setTexture(Texture_Manager::get("VEHICLE0"));
    sf::Vector2i tpos;
    tpos.x = static_cast<int>(type) * 64;
    tpos.y = 0;
    sf::Vector2i tsize(64, 64);
    setTextureRect(sf::IntRect(tpos, tsize));
    setOrigin(sf::Vector2f(getGlobalBounds().width / 2.f, getGlobalBounds().height / 2.f));
    */
}

Vehicle::Type Vehicle::stringToType(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "BOAT") {
        return BOAT;
    }
    else if (s == "BROOM") {
        return BROOM;
    }
    else {
        return NULL_TYPE;
    }
}
