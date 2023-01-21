#include <animation/animation.hpp>

//////////////////////////////////////////////////////////////

Animation::Animation(sf::Vector2i start, sf::Vector2i size, unsigned int frameCount)
{
    for (unsigned int f = 0; f < frameCount; ++f) {
        frames.push_back(sf::IntRect(start, size));
        start.x += std::abs(size.x);
    }
}

sf::IntRect Animation::nextFrame()
{
    if (!repeats && currentFrame == frames.size() - 1) {
        return frames[currentFrame];
    }
    currentFrame++;
    if (currentFrame >= frames.size()) {

        currentFrame = 0;
    }

    return frames[currentFrame];
}

void Animation::reset()
{
    currentFrame = 0;
}

sf::IntRect Animation::firstFrame()
{
    reset();
    return frames[0];
}

unsigned int Animation::getCurrentFrame()
{
    return currentFrame;
}

bool Animation::lastFrame()
{
    return (currentFrame == frames.size() - 1);
}

unsigned int Animation::transitionFrom()
{
    unsigned int f = currentFrame;
    reset();
    return f;
}

sf::IntRect Animation::transitionTo(unsigned int frame)
{
    currentFrame = frame;
    //if it's crashing here, double check the database...
    return frames[currentFrame];
}
