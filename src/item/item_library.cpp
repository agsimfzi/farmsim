#include <item/item_library.hpp>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <util/vector2_stream.hpp>

Item_Library::Item_Library()
{
    std::vector<Item_Data> items = Database::getItemPrototypes();
    for (auto& item : items) {
        Item i(item);
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
        }

        size_t sheet_id = item.uid % 1000;

        texture += std::to_string(sheet_id / 100);

        sprite.setTexture(Texture_Manager::get(texture));

        sf::Vector2i pos;
            pos.x = (sheet_id % 10) * 64;
            pos.y = (sheet_id / 10) * 64;
        sf::Vector2i size(64, 64);
        sprite.setTextureRect(sf::IntRect(pos, size));
        i.setSprite(sprite);

        std::shared_ptr<Item> iptr = std::make_shared<Item>(i);

        stringShelf[item.name] = iptr;
        uidShelf[item.uid] = iptr;

        std::cout << "item " << item.uid << " added, with texture " << texture << '\n';
        std::cout << "\ttextureRect of " << pos << " : " << size << '\n';
    }
}

Item* Item_Library::item(std::string name)
{
    return stringShelf[name].get();
}

Item* Item_Library::item(unsigned int uid)
{
    return uidShelf[uid].get();
}
