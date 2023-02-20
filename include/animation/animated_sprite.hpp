#pragma once

#include <SFML/Graphics.hpp>

#include <SFML/System/Clock.hpp>

#include <map>

#include <entity/entity_state.hpp>

#include <world/direction.hpp>

#include "animation/animation.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
template <typename K>
class Animated_Sprite : public sf::Sprite {
public:
    Animated_Sprite() = default;

    Animated_Sprite(sf::Texture& ntexture,
        sf::Vector2i nsize,
        std::map<K, unsigned int> counts,
        std::map<K, int> thresholds)
        : size { nsize }
    {
        loadCounts(counts);
        loadThresholds(thresholds);
        setTexture(ntexture);
        direction = Direction::N;
        setAnimationState(static_cast<K>(0));
        setOrigin(sf::Vector2f(size) / 2.f);
        updateFrame();
    }

    void setAnimationState(K nstate)
    {
        if (state != nstate) {
            animations[state][direction].reset();
            state = nstate;
            setTextureRect(animations[state][direction].firstFrame());
            frameThreshold = animations[state][direction].threshold;
            frameTimer.restart();
        }
    }

    K getState()
    {
        return state;
    }

    void setDirection(Direction ndirection)
    {
        if (direction != ndirection) {
            setTextureRect(animations[state][ndirection].transitionTo(animations[state][direction].transitionFrom()));
            direction = ndirection;
        }
    }

    Direction getDirection()
    {
        return direction;
    }

    void update()
    {
        if (frameTimer.getElapsedTime().asMilliseconds() >= frameThreshold) {
            frameTimer.restart();
            updateFrame();
        }
    }

    bool done()
    {
        return (lastFrame() && frameTimer.getElapsedTime().asMilliseconds() >= frameThreshold);
    }

    bool lastFrame()
    {
        return animations[state][direction].lastFrame();
    }

    sf::Vector2f getSize()
    {
        sf::Vector2f size;
        size.x = getGlobalBounds().width;
        size.y = getGlobalBounds().height;
        return size;
    }

private:
    sf::Vector2i size; /**< frame size for setTextureRect()   */

    int animationY {}; /**< offset of specific animation from the start of the spritesheet    */
    int directionY {}; /**< offset of specific direction from animationY                      */

    K state {};
    Direction direction {};

    sf::Clock frameTimer;
    int frameThreshold { 250 };

    std::map<K, std::map<Direction, Animation>> animations;

    void loadCounts(std::map<K, unsigned int> times)
    {
        const int dlimit = static_cast<int>(Direction::NULL_DIRECTION);
        for (auto& t : times) {
            for (unsigned int i = 0; i < dlimit; ++i) {
                sf::Vector2i start(0, static_cast<int>(t.first) * (size.y * 5));
                Direction d = static_cast<Direction>(i);

                sf::Vector2i aSize = size;

                int dFactor = 1;

                if (d > Direction::S) {
                    aSize.x *= -1;
                    start.x += size.x;
                    dFactor = static_cast<int>(mirrorDirection(d));
                }
                else
                    dFactor = static_cast<int>(d);

                start.y += (dFactor * size.y);

                animations[t.first][d] = Animation(start, aSize, t.second);
            }
        }
    }

    void loadThresholds(std::map<K, int> thresholds)
    {
        for (auto& a : animations) {
            for (auto& b : a.second) {
                b.second.threshold = thresholds[a.first];
            }
        }
    }

    void updateFrame()
    {
        setTextureRect(animations[state][direction].nextFrame());
    }
};
