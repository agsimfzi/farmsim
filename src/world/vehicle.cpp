#include <world/vehicle.hpp>

#include <resources/texture_manager.hpp>

Vehicle::Vehicle(sf::Vector2f pos, Vehicle_Data d)
    : Animated_Sprite<Vehicle_State>(Texture_Manager::get(d.name), d.size, d.counts, d.thresholds)
    , type{ stringToType(d.name) }
    , speed_factor{ d.speed_factor }
{
    setPosition(pos);
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
