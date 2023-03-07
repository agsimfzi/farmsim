#include <animation/animation.hpp>

//////////////////////////////////////////////////////////////

Animation::Animation(sf::Vector2i start, sf::Vector2i size, unsigned int frame_count)
{
    last_frame = frame_count - 1;
    for (unsigned int f = 0; f < frame_count; ++f) {
        frames.push_back(sf::IntRect(start, size));
        start.x += std::abs(size.x);
    }
}

sf::IntRect Animation::nextFrame()
{
    if (repeats || current_frame < last_frame) {
        current_frame++;
        if (current_frame > last_frame) {

            current_frame = 0;
        }
    }

    return frames[current_frame];
}

void Animation::reset()
{
    current_frame = 0;
}

sf::IntRect Animation::firstFrame()
{
    reset();
    return frames[0];
}

bool Animation::lastFrame()
{
    return (current_frame == last_frame);
}

unsigned int Animation::transitionFrom()
{
    unsigned int f = current_frame;
    reset();
    return f;
}

sf::IntRect Animation::transitionTo(unsigned int frame)
{
    current_frame = frame;
    // if it's crashing here, double check the database...
    return frames[current_frame];
}
