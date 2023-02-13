#include <item/item_library.hpp>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <util/vector2_stream.hpp>

Item_Library::Item_Library()
{
    std::vector<Item_Data> items = Database::getItemPrototypes();
    for (auto& item : items) {
        Item i(item);
        i.setCount(1);
        sf::Sprite sprite;
        sf::IntRect textureRect;

        std::string texture;

        switch (item.uid / 1000) {
            case 0:
                texture = "ITEMS";
                break;
            case 1:
                texture = "SEEDS";
                break;
            case 2:
                texture = "PLANTS";
                break;
            case 3:
                texture = "CONTAINER";
                break;
            case 4:
                texture = "FURNITURE";
                break;
            case 5:
                texture = "MACHINE";
                break;
            case 6:
                texture = "LOOTABLE";
                break;
            default:
                texture = "ITEMS";
                break;
        }

        size_t sheet_id = item.uid % 1000;

        texture += std::to_string(sheet_id / 100);

        sheet_id %= 100;

        sprite.setTexture(Texture_Manager::get(texture));

        sf::Vector2i pos;
        pos.x = (sheet_id % 10) * 64;
        pos.y = (sheet_id / 10) * 64;
        sf::Vector2i size(64, 64);
        sprite.setTextureRect(sf::IntRect(pos, size));
        sprite.setOrigin(sf::Vector2f(size) / 2.f);
        i.setSprite(sprite);

        std::shared_ptr<Item> iptr = std::make_shared<Item>(i);

        stringShelf[item.name] = iptr;
        uidShelf[item.uid] = iptr;

        std::cout << "item " << item.name << " (" << item.uid << ") added, subtype " << item.subtype
                  << ", use factor " << item.use_factor
                  << " from sheet " << texture << " ("
                  << pos << " x " << size << ")\n";
    }
}

Item* Item_Library::item(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    return stringShelf[name].get();
}

Item* Item_Library::item(size_t uid)
{
    return uidShelf[uid].get();
}

std::shared_ptr<Item> Item_Library::shared(size_t uid)
{
    return std::make_shared<Item>(*uidShelf[uid]);
}
std::shared_ptr<Item> Item_Library::shared(std::string name)
{
    return std::make_shared<Item>(*stringShelf[name]);
}
