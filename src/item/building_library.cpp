#include <item/building_library.hpp>

#include <system/database.hpp>

#include <util/vector2_stream.hpp>

Building_Library::Building_Library()
{
    std::map<Machine_Type, std::vector<Reaction>> reactions = Database::getReactions();
    std::vector<Reaction> seed_reactions;
    std::map<Crafting_Type, std::vector<Reaction>> recipes = Database::getRecipes();
    std::vector<Item_Data> items = Database::getItemPrototypes();
    for (auto& item : items) {
        if (item.type == Item_Type::SEED) {
            seed_reactions.push_back(generateSeedReaction(item));
        }
        if (item.type == Item_Type::BUILDING) {
            std::shared_ptr<Machine> m;
            std::shared_ptr<Building> b;
            Building::Type t = findBuildingType(item.subtype);
            std::string derived_subtype; // for buildings with complex substrings (crafting/machines)
            switch (t) {
                case Building::CONTAINER:
                    b = std::make_shared<Container>();
                    break;
                case Building::CRAFTING:
                    b = std::make_shared<Crafting>();
                    derived_subtype = item.subtype.substr(item.subtype.find(':') + 1);
                    b->reactions = recipes[stringToCraftingType(derived_subtype)];
                    item.subtype = derived_subtype;
                    break;
                case Building::FURNITURE:
                    b = std::make_shared<Furniture>();
                    break;
                case Building::MACHINE:
                    m = std::make_shared<Machine>();
                    derived_subtype = item.subtype.substr(item.subtype.find(':') + 1);
                    m->machine_type = stringToMachineType(derived_subtype);
                    if (m->machine_type == Machine_Type::SEED_EXTRACTOR) {
                        std::sort(seed_reactions.begin(), seed_reactions.end(),
                            [] (Reaction& l, Reaction& r) -> bool { return l < r; });
                        m->reactions = seed_reactions;
                    }
                    else {
                        m->reactions = reactions[stringToMachineType(derived_subtype)];
                    }
                    m->countReagants();
                    item.subtype = derived_subtype;
                    b = m;
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

            uidShelf[b->uid] = b;
            stringShelf[b->name] = b;
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
