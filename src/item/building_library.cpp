#include <item/building_library.hpp>

#include <system/database.hpp>

#include <util/vector2_stream.hpp>

Building_Library::Building_Library()
{
    std::map<Machine_Type, std::vector<Reaction>> reactions = Database::getReactions();
    std::vector<Item_Data> items = Database::getItemPrototypes();
    for (auto& item : items) {
        if (item.type == Item_Type::BUILDING) {
            std::shared_ptr<Building> b;
            switch (findBuildingType(item)) {
                case Building::CONTAINER:
                    b = std::make_shared<Container>();
                    break;
                case Building::FURNITURE:
                    //b = std::make_shared<Furniture>();
                    break;
                case Building::MACHINE:
                    b = std::make_shared<Machine>();
                    break;
                case Building::LOOTABLE:
                    b = std::make_shared<Lootable>();
                    break;
                case Building::STRUCTURE:
                    //b = std::make_shared<Structure>();
                    break;
                default:
                    b = std::make_shared<Building>();
                    break;
            }

            b->name = item.name;
            b->uid = item.uid;
            if (item.subtype.find("MACHINE") == std::string::npos) {
                b->type = Building::stringToType(item.subtype);
            }
            else {
                b->type = Building::MACHINE;
                std::string mts = item.subtype.substr(item.subtype.find(':') + 1);
                Machine_Type mtype = stringToMachineType(mts);
                b->reactions = reactions[mtype];
            }

            std::cout << "LIBRARY ADD, building " << b->name << " (" << b->uid << ") added, type " << item.subtype
                      << "\n\treactions:\n";
            for (const auto& r : b->reactions) {
                std::cout << "\t\t" << r.name << "\n\t\t";
                for (const auto& n : r.reagants) {
                    std::cout << ", " << n;
                }
                std::cout << "\n\t -> " << r.product << ", length " << r.length << '\n';
            }

            uidShelf[b->uid] = b;
            stringShelf[b->name] = b;

        }
    }
}

Building::Type Building_Library::findBuildingType(Item_Data i)
{
    std::string t = i.subtype;

    if (t.find("MACHINE") != std::string::npos) {
        return Building::MACHINE;
    }

    return Building::stringToType(t);
}
