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
    if (!fully_grown) {
        float stage_factor = growth_coef + (growth_coef * watered);
        growth += stage_factor;

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
    return fully_grown;
}

size_t Crop::getUID()
{
    return uid;
}

void Crop::nextStage()
{
    if (!fully_grown) {
        sf::IntRect tr = sprite.getTextureRect();
        tr.left += tr.width;
        sprite.setTextureRect(tr);
        growth = 0.f;
        if (++stage == 3) {
            fully_grown = true;
        }
    }
}

void Crop::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
