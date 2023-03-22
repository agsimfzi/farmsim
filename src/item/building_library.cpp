#include <item/building_library.hpp>

#include <system/database.hpp>

#include <world/building_data.hpp>

#include <util/primordial.hpp>
#include <util/vector2_stream.hpp>

Building_Library::Building_Library()
{
    std::map<Machine_Type, std::vector<Reaction>> reactions = Database::getReactions();
    std::map<Crafting_Type, std::vector<Reaction>> recipes = Database::getRecipes();
    std::map<size_t, Building_Animation_Data> animation_data = Database::getBuildingAnimationData();
    std::vector<Item_Data> items = Database::getItemPrototypes();
    for (auto& item : items) {
        if (item.type == Item_Type::SEED) {
            reactions[Machine_Type::SEED_EXTRACTOR].push_back(generateSeedReaction(item));
        }
        else if (equalStrings(item.subtype, "WOOD")) {
            reactions[Machine_Type::TABLE_SAW].push_back(generateWoodReaction(item));
        }
        if (item.type == Item_Type::BUILDING) {
            std::shared_ptr<Machine> m;
            std::shared_ptr<Building> b;
            Building::Type t = findBuildingType(item.subtype);
            Building_Animation_Data ad = animation_data[item.uid];
            std::string derived_subtype; // for buildings with complex substrings (crafting/machines)
            ad.tkey = Building::typeToString(t) + std::to_string((item.uid % 1000) / 100) + "-WORLD";
            switch (t) {
                case Building::CONTAINER:
                    b = std::make_shared<Container>(ad);
                    break;
                case Building::CRAFTING:
                    b = std::make_shared<Crafting>(ad);
                    derived_subtype = item.subtype.substr(item.subtype.find(':') + 1);
                    b->reactions = recipes[stringToCraftingType(derived_subtype)];
                    item.subtype = derived_subtype;
                    break;
                case Building::FURNITURE:
                    b = std::make_shared<Furniture>(ad);
                    break;
                case Building::MACHINE:
                    m = std::make_shared<Machine>(ad);
                    derived_subtype = item.subtype.substr(item.subtype.find(':') + 1);
                    m->machine_type = stringToMachineType(derived_subtype);
                    m->reactions = reactions[stringToMachineType(derived_subtype)];
                    m->countReagants();
                    item.subtype = derived_subtype;
                    b = m;
                    break;
                case Building::LOOTABLE:
                    b = std::make_shared<Lootable>(ad);
                    break;
                case Building::STRUCTURE:
                    //b = std::make_shared<Structure>(ad);
                    break;
                default:
                    b = std::make_shared<Building>(ad);
                    break;
            }

            b->type = t;
            b->name = item.name;
            b->uid = item.uid;
            b->subtype = item.subtype;

            std::cout << "LIBRARY ADD, building " << b->name << " (" << b->uid << ") added, type " << item.subtype
                      << "\n\treactions:\n";
            for (const auto& r : b->reactions) {
                std::cout << "\t\t" << r.name << "\n\t\t";
                for (const auto& n : r.reagants) {
                    std::cout << ", " << n.count << " " << n.name;
                }
                std::cout << "\n\t -> " << r.product << ", length " << r.length << '\n';
            }

            uid_shelf[b->uid] = b;
            string_shelf[b->name] = b;
        }
    } // item loop
}

std::shared_ptr<Building> Building_Library::makeBySubtype(Building* b)
{
    switch (b->type) {
        case Building::CONTAINER:
            return std::make_shared<Container>(*dynamic_cast<Container*>(b));
        case Building::CRAFTING:
            return std::make_shared<Crafting>(*dynamic_cast<Crafting*>(b));
        case Building::FURNITURE:
            return std::make_shared<Furniture>(*dynamic_cast<Furniture*>(b));
        case Building::MACHINE:
            return std::make_shared<Machine>(*dynamic_cast<Machine*>(b));
        case Building::LOOTABLE:
            return std::make_shared<Lootable>(*dynamic_cast<Lootable*>(b));
        case Building::STRUCTURE:
            //return std::make_shared<Structure>(*dynamic_cast<Structure*>(b));
        default:
            return std::make_shared<Building>(*dynamic_cast<Building*>(b));
    }
}

Building::Type Building_Library::findBuildingType(std::string subtype)
{
    if (subtype.find("MACHINE") != std::string::npos) {
        return Building::MACHINE;
    }
    else if (subtype.find("CRAFTING") != std::string::npos) {
        return Building::CRAFTING;
    }

    return Building::stringToType(subtype);
}

Reaction Building_Library::generateSeedReaction(Item_Data item)
{
    Reaction r;
    r.name = "Extract " + item.name;
    Reagant rgt;
    rgt.count = 1;
    rgt.name = item.name;
    std::string extract = " Seeds";
    rgt.name.resize(item.name.size() - extract.length());
    r.reagants.push_back(rgt);
    r.product = item.name;
    r.length = std::stoi(item.subtype.substr(0, item.subtype.find(';')));
    r.tag = item.subtype.substr(item.subtype.find(';') + 1);
    return r;
}

Reaction Building_Library::generateWoodReaction(Item_Data item)
{
    Reaction r;
    std::string variant = item.name.substr(0, item.name.find(' '));
    std::string product = variant + " plank";
    r.product = product;
    r.name = "Make " + product;
    Reagant rgt;
    rgt.count = 1;
    rgt.name = item.name;
    r.reagants.push_back(rgt);
    r.length = item.use_factor;
    r.tag = 1;
    return r;
}
