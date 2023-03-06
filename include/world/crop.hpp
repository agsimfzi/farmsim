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
    size_t harvestUID();

    void place(sf::Vector2i coordinates, sf::Vector2f pos);

    bool checkSeason(Season s);

    void kill();
    bool dead();

    std::function<void(sf::Vector2i)> unwater;

    unsigned int getQuantity();

    void harvestRegrowable();

    bool passable();
    bool regrows();

private:
    sf::Sprite sprite;

    float growth = 0.f;
    const static float stage_threshold;

    sf::Vector2i coordinates;

    size_t stage = 1;

    void nextStage();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
