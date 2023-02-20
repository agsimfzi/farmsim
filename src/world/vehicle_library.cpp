#include <world/vehicle_library.hpp>

#include <system/database.hpp>

Vehicle_Library::Vehicle_Library()
{
    std::vector<Vehicle_Data> data = Database::getVehicles();

    for (auto d : data) {
        Vehicle::Type t = Vehicle::stringToType(d.name);
        v[t] = d;
        // v.insert(std::make_pair(t, d));
    }
}
