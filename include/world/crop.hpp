#pragma once

#include <SFML/Graphics.hpp>

#include "crop_data.hpp"

class Crop : public sf::Drawable, private Crop_Data {
public:
    Crop() = default;
    Crop(Crop_Data d);

    void setSprite(sf::Sprite sprite);
    sf::Sprite& getSprite();

    void tick(bool watered);

    sf::Vector2i getCoordinates();

    bool fullyGrown();

    size_t getUID();

private:
    sf::Sprite sprite;

    float growth = 0.f;
    const static float stage_threshold;

    unsigned int stage = 0;

    void nextStage();

    bool fully_grown = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
