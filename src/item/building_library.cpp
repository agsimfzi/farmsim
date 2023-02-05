#include <item/building_library.hpp>

#include <system/database.hpp>

#include <util/vector2_stream.hpp>

Building_Library::Building_Library()
{
    std::map<Building::Type, std::vector<Reaction>> reactions = Database::getReactions();
    std::vector<Item_Data> items = Database::getItemPrototypes();
    for (auto& item : items) {
        if (item.type == Item_Type::BUILDING) {
            Building b;
            b.name = item.name;
            b.uid = item.uid;
            b.type = Building::stringToType(item.subtype);
            b.reactions = reactions[b.type];

            std::shared_ptr<Building> bp = std::make_shared<Building>(b);

            uidShelf[b.uid] = bp;
            stringShelf[b.name] = bp;

            std::cout << "LIBRARY ADD, building " << b.name << " (" << b.uid << ") added, type " << item.subtype
                      << "\n\treactions:\n";
            for (const auto& r : reactions[b.type]) {
                std::cout << "\t\t" << r.name << ", "
                          << r.reagant << " -> " << r.product << ", length " << r.length << '\n';
            }

        }
    }
}
