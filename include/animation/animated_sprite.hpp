#pragma once

#include <SFML/Graphics.hpp>

#include <SFML/System/Clock.hpp>

#include <map>

#include <world/direction.hpp>

#include "animation/animation.hpp"

/// Animated_Sprite ///
/// \brief A template class for animated sf::Sprite objects. The template type is the state.
///
template <typename K>
class Animated_Sprite : public sf::Sprite {
public:
    Animated_Sprite() = default;

/// FULL CONSTRUCTOR ///
/// \brief initializes the sprite using animation data and textureRect parameters
///
/// \param size: textureRect size
/// \param counts: the number of frames per animation state
/// \param thresholds: the length of each frame per animation state
/// \param start: initial textureRect position
/// \param directionless: determines whether direction is factored in to a sprite. for calculating state-derived offsets.
///
    Animated_Sprite(sf::Texture& ntexture,
        sf::Vector2i size,
        std::map<K, unsigned int> counts,
        std::map<K, int> thresholds
        , sf::Vector2i start = sf::Vector2i(0, 0)
        , bool directionless = false)
        : size { size }
        , start { start }
    {
        if (!directionless) {
            loadCounts(counts);
        }
        else {
            loadDirectionlessCounts(counts);
        }
        loadThresholds(thresholds);
        setTexture(ntexture);
        direction = Direction::N;
        setAnimationState(static_cast<K>(0));
        setOrigin(sf::Vector2f(size) / 2.f);
        updateFrame();
    }

/// setAnimationState ///
/// \brief applies a new animation state, resetting the old state and restarting the frame timer.
///
/// \param state: the new state
///
    void setAnimationState(K state)
    {
        if (this->state != state) {
            animations[state][direction].reset();
            this->state = state;
            setTextureRect(animations[state][direction].firstFrame());
            frame_threshold = animations[state][direction].threshold;
            frame_timer.restart();
        }
    }

/// getState ///
/// \brief returns the active state
///
/// \return state
///
    K getState()
    {
        return state;
    }

/// setDirection ///
/// \brief changes the current sprite direction, transitioning the animation in the process.
///
/// \param direction: the new direction
///
    void setDirection(Direction direction)
    {
        if (this->direction != direction) {
            setTextureRect(animations[state][direction].transitionTo(animations[state][this->direction].transitionFrom()));
            this->direction = direction;
        }
    }

/// getDirection ///
/// \brief returns the current sprite direction.
///
/// \return current direction
///
    Direction getDirection()
    {
        return direction;
    }

/// update ///
/// \brief checks that the frame_timer has reached its threshold and, if so, advances the animation
///
    void update()
    {
        if (frame_timer.getElapsedTime().asMilliseconds() >= frame_threshold) {
            frame_timer.restart();
            updateFrame();
        }
    }

/// getSize ///
/// \brief returns the textureRect's size
///
/// \param globalBounds -> size
///
    sf::Vector2f getSize()
    {
        sf::Vector2f size;
        size.x = getGlobalBounds().width;
        size.y = getGlobalBounds().height;
        return size;
    }

/// unsetRepeat ///
/// \brief Flags the provided state as non-repeating
///
/// \param k: state to flag
///
    void unsetRepeat(K k) {
        for (auto& d : animations[k]) {
            d.second.repeats = false;
        }
    }

/// done ///
/// \brief Checks that a non-repeating animation is complete
///
/// \return lastFrame and threshold exceeded
///
    bool done()
    {
        return (lastFrame() && frame_timer.getElapsedTime().asMilliseconds() >= frame_threshold);
    }

/// lastFrame ///
/// \brief Animation::lastFrame() for current state
///
/// \return current state frame is last frame
///
    bool lastFrame()
    {
        return animations[state][direction].lastFrame();
    }

private:
    sf::Vector2i size; /**< textureRect size */
    sf::Vector2i start; /**< initial textureRect position */

    K state {}; /**< current sprite state */
    Direction direction {}; /**< current sprite direction */

    sf::Clock frame_timer; /**<  Tracks the time since the start of the current frame. */
    int frame_threshold { 250 }; /**< Tracks the time at which the next frame is loaded */

    std::map<K, std::map<Direction, Animation>> animations; /**< Animations by state and direction */

/// loadCounts ///
/// \brief Loads the frame count for each animation state.
///
/// \param counts: std::map keyed to state, containing unsigned integer frame counts.
///
    void loadCounts(std::map<K, unsigned int> counts)
    {
        const int dlimit = static_cast<int>(Direction::NULL_DIRECTION);
        for (auto& t : counts) {
            for (unsigned int i = 0; i < dlimit; ++i) {
                sf::Vector2i pos(0, static_cast<int>(t.first) * (size.y * 5));
                Direction d = static_cast<Direction>(i);

                sf::Vector2i a_size = size;

                int dFactor = 1;

                if (d > Direction::S) {
                    a_size.x *= -1;
                    pos.x += size.x;
                    dFactor = static_cast<int>(mirrorDirection(d));
                }
                else
                    dFactor = static_cast<int>(d);

                pos.y += (dFactor * size.y);

                pos += start;

                animations[t.first][d] = Animation(pos, a_size, t.second);
            }
        }
    }

/// loadDirectionlessCounts ///
/// \brief Loads the frame count for each animation state in a directionless context.
///
/// \param counts: std::map keyed to state, containing unsigned integer frame counts.
///
    void loadDirectionlessCounts(std::map<K, unsigned int> counts)
    {
        const int dlimit = static_cast<int>(Direction::NULL_DIRECTION);
        for (auto& t : counts) {
            sf::Vector2i pos(start);
            pos.y += static_cast<int>(t.first) * size.y;
            for (unsigned int i = 0; i < dlimit; ++i) {
                Direction d = static_cast<Direction>(i);
                animations[t.first][d] = Animation(pos, size, t.second);
            }
        }
    }

/// loadThresholds ///
/// \brief Loads the frame time thresholds for each animation state.
///
/// \param counts: std::map keyed to state, containing integer frame thresholds.
///
    void loadThresholds(std::map<K, int> thresholds)
    {
        for (auto& a : animations) {
            for (auto& b : a.second) {
                b.second.threshold = thresholds[a.first];
            }
        }
    }

/// loadDirectionlessCounts ///
/// \brief retrieves and applies the next animation frame
///
    void updateFrame()
    {
        setTextureRect(animations[state][direction].nextFrame());
    }
};
