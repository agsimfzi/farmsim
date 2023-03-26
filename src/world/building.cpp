#include <world/building.hpp>

#include <resources/texture_manager.hpp>

#include <util/primordial.hpp> // for equalStrings

#include <world/tile.hpp>

Building::Building(Building_Animation_Data ad)
{
    sf::Vector2i size(48, 64);
    sprite = Animated_Sprite<Building_State>(Texture_Manager::get(ad.tkey), size, ad.counts, ad.thresholds, ad.start, true);
    sf::Vector2f origin;
    origin.x = size.x / 2;
    origin.y = size.y - (tile_size / 2);
    sprite.setOrigin(origin);
    sprite.unsetRepeat(Building_State::IDLE);
    sprite.unsetRepeat(Building_State::STARTING);
    sprite.unsetRepeat(Building_State::ENDING);
    sprite.setAnimationState(Building_State::IDLE);
    sprite.update();
}

std::vector<std::vector<std::shared_ptr<Item>>>& Building::getInventory()
{
    return inventory;
}

bool Building::empty()
{
    for (const auto& r : inventory) {
        for (const auto& c : r) {
            if (c) {
                return false;
            }
        }
    }

    return true;
}

void Building::updateSprite()
{
    Building_State s = sprite.getState();
    switch (s) {
        case Building_State::STARTING:
            if (sprite.done()) {
                sprite.setAnimationState(Building_State::RUNNING);
            }
            break;
        case Building_State::ENDING:
            if (sprite.done()) {
                sprite.setAnimationState(Building_State::IDLE);
            }
            break;
        default:
            break;
    }
    sprite.update();
}
