#include <world/crop.hpp>

#include <iostream>

const float Crop::stage_threshold = 1.f;

Crop::Crop(Crop_Data d)
    : Crop_Data(d)
{
}

void Crop::setSprite(sf::Sprite sprite)
{
    this->sprite = sprite;
}

sf::Sprite& Crop::getSprite()
{
    return sprite;
}

void Crop::tick(bool watered)
{
    // introduce random variance
    if (!fullyGrown()) {
        float stage_advance = growth_coef;
        if (watered) {
            stage_advance *= water_factor;
        }
        growth += stage_advance;
        if (growth >= stage_threshold) {
            nextStage();
        }
    }
}

sf::Vector2i Crop::getCoordinates()
{
    return coordinates;
}

bool Crop::fullyGrown()
{
    return (stage >= stage_count);
}

size_t Crop::getUID()
{
    return uid;
}

size_t Crop::harvestUID()
{
    return uid + 1000;
}

void Crop::place(sf::Vector2i coordinates, sf::Vector2f pos)
{
    this->coordinates = coordinates;
    sprite.setPosition(pos);
}

void Crop::nextStage()
{
    if (!fullyGrown()) {
        sf::IntRect tr = sprite.getTextureRect();
        tr.top += y_size;
        sprite.setTextureRect(tr);
        growth = 0.f;
        stage++;
    }
}

void Crop::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
