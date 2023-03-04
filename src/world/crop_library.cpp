#include <world/crop_library.hpp>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <world/tile.hpp>

#include <util/vector2_stream.hpp>

Crop_Library::Crop_Library()
{
    std::cout << "\n\ninitializing crop library...\n";
    std::vector<Crop_Data> crop_data = Database::getCropPrototypes();
    for (auto& d : crop_data) {
        Crop c(d);
        sf::Sprite sprite;
        sf::IntRect textureRect;

        std::string texture = "CROPS";
        size_t sheet_id = (d.uid % 1000) / 100;
        texture += std::to_string(sheet_id);


        sprite.setTexture(Texture_Manager::get(texture));

        sf::Vector2i pos;
        sf::Vector2i size(48, d.y_size);
        pos.x = (d.uid % 100) * size.x;
        pos.y = 0;
        sprite.setTextureRect(sf::IntRect(pos, size));
        sf::Vector2f origin;
        origin.x = size.x / 2;
        origin.y = size.y - (Tile::tile_size / 2.f);
        sprite.setOrigin(origin);
        c.setSprite(sprite);

        //std::unique_ptr<Item> iptr =

        shelf[d.uid] = std::make_unique<Crop>(c);
        std::cout << "crop " << d.name << " added to library, uid " << d.uid
                  << ", growth " << d.growth_coef
                  << ", water " << d.water_factor
                  << ", stages " << d.stage_count
                  << ", size " << d.y_size << '\n';
        std::cout << "\tspring: " << d.seasons[Season::SPRING] << '\n';
        std::cout << "\tsummer: " << d.seasons[Season::SUMMER] << '\n';
        std::cout << "\tautumn: " << d.seasons[Season::AUTUMN] << '\n';
        std::cout << "\twinter: " << d.seasons[Season::WINTER] << '\n';
    }
}

Crop* Crop_Library::get(size_t uid)
{
    if (!shelf.contains(uid)) {
        std::cout << "FAILED TO FIND CROP " << uid << "!\n";
    }
    return shelf[uid].get();
}
