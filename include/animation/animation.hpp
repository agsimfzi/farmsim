#pragma once

#include <SFML/Graphics.hpp>

/////////////////////////////////////////////////////////////
/// \brief
///
class Animation {
public:
    Animation() { }
    Animation(sf::Vector2i start, sf::Vector2i size, unsigned int frameCount);
    sf::IntRect nextFrame();

    bool repeats = true;

    void reset();

    sf::IntRect firstFrame();

    unsigned int getCurrentFrame();

    bool lastFrame();

    float threshold;

    unsigned int transitionFrom();
    sf::IntRect transitionTo(unsigned int frame);

private:
    unsigned int currentFrame = 0;
    std::vector<sf::IntRect> frames;
};
