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
class Animated_Sprite : public sf::Sprite {
public:
    Animated_Sprite() { }
    Animated_Sprite(sf::Texture& ntexture,
        sf::Vector2i nsize,
        std::map<Entity_State, unsigned int> counts,
        std::map<Entity_State, int> thresholds);

    void setAnimationState(Entity_State nstate);
    Entity_State getAnimationState();

    void setDirection(Direction ndirection);
    Direction getDirection();

    void update();

    Entity_State getState();

    bool done();

    bool lastFrame();

    sf::Vector2f getSize();

private:
    void loadCounts(std::map<Entity_State, unsigned int> counts);
    void loadThresholds(std::map<Entity_State, int> thresholds);

    sf::Vector2i size; /**< frame size for setTextureRect()   */

    int animationY {}; /**< offset of specific animation from the start of the spritesheet    */
    int directionY {}; /**< offset of specific direction from animationY                      */

    Entity_State state {};
    Direction direction {};

    sf::Clock frameTimer;
    int frameThreshold { 250 };

    void updateFrame();

    std::map<Entity_State, std::map<Direction, Animation>> animations;
};
