#include <world/detail_library.hpp>

#include <system/database.hpp>

Detail_Library::Detail_Library()
{
    std::vector<Detail_Data> data = Database::getDetails();
    sf::Vector2i c(0, 0); // placeholder coordinates
    for (auto& d : data) {
        shelf[d.name] = Detail(c, d);
    }
}
