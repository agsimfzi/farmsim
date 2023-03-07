#pragma once

#include <SFML/Graphics.hpp>


/////////////////////////////////////////////////////////////
/// \brief
///
class Animation {
public:
    Animation() = default;

/// FULL CONSTRUCTOR ///
/// \brief defines the textureRect for each frame
///
/// \param start: first textureRect position
/// \param size: first textureRect size
/// \param frame_count: number of textureRects to define
///
    Animation(sf::Vector2i start, sf::Vector2i size, unsigned int frame_count);

/// nextFrame ///
/// \brief defines the textureRect for each frame
///
/// \returns frames[++current_frame]
/// (at end, returns last frame again OR wraps around to zero based on repeating)
///
    sf::IntRect nextFrame();

/// reset ///
/// \brief sets the current_frame to zero
///
    void reset();

/// firstFrame ///
/// \brief defines the textureRect for each frame
///
/// \param start: first textureRect position
/// \param size: first textureRect size
/// \param frame_count: number of textureRects to define
///
    sf::IntRect firstFrame();

/// lastFrame ///
/// \brief defines the textureRect for each frame
///
/// \return current_frame == last_frame
///
    bool lastFrame();

/// transitionFrom ///
/// \brief provides the current_frame for non-interrupting animation changes.
/// primarily intended for direction change
///
/// \return current_frame
///
    unsigned int transitionFrom();

/// transitionTo ///
/// \brief reads and applies the frame index provided by the transitionFrom for non-interrupting animation changes
/// primarily intended for direction change
///
/// \param frame:
/// \return frames[frame]
///
    sf::IntRect transitionTo(unsigned int frame);

    bool repeats = true; /**< Right what it says on the tin. */

    float threshold; /**< Defines the length of each frame. */

private:
    unsigned int current_frame = 0; /**<  Index of the active animation frame. */
    unsigned int last_frame = 0; /**<  Final frame of the animation. */
    std::vector<sf::IntRect> frames; /**<  textureRect */
};
