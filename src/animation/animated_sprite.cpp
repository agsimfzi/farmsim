#include <animation/animated_sprite.hpp>

//////////////////////////////////////////////////////////////

Animated_Sprite::Animated_Sprite(sf::Texture& ntexture,
    sf::Vector2i nsize,
    std::map<Entity_State, unsigned int> counts,
    std::map<Entity_State, int> thresholds)
    : size { nsize }
{
    loadCounts(counts);
    loadThresholds(thresholds);
    setTexture(ntexture);
    direction = Direction::N;
    setAnimationState(Entity_State::IDLE);
    setOrigin(sf::Vector2f(size) / 2.f);

    blip.setPointCount(4);
    blip.setRadius(4);
    blip.setFillColor(sf::Color::Red);
    blip.setOrigin(sf::Vector2f(2, 2));
}

void Animated_Sprite::setAnimationState(Entity_State nstate)
{
    if (state != nstate) {
        animations[state][direction].reset();
        state = nstate;
        setTextureRect(animations[state][direction].firstFrame());
        frameThreshold = animations[state][direction].threshold;
        frameTimer.restart();
    }
}

void Animated_Sprite::setDirection(Direction ndirection)
{
    if (direction != ndirection) {
        setTextureRect(animations[state][ndirection].transitionTo(animations[state][direction].transitionFrom()));
        direction = ndirection;
    }
}

Direction Animated_Sprite::getDirection()
{
    return direction;
}

void Animated_Sprite::update()
{
    blip.setPosition(getPosition());
    if (frameTimer.getElapsedTime().asMilliseconds() >= frameThreshold) {
        frameTimer.restart();
        updateFrame();
    }
}

void Animated_Sprite::updateFrame()
{
    setTextureRect(animations[state][direction].nextFrame());
}

Entity_State Animated_Sprite::getAnimationState()
{
    return state;
}

void Animated_Sprite::loadCounts(std::map<Entity_State, unsigned int> times)
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

void Animated_Sprite::loadThresholds(std::map<Entity_State, int> thresholds)
{
    for (auto& a : animations) {
        for (auto& b : a.second) {
            b.second.threshold = thresholds[a.first];
        }
    }
}

Entity_State Animated_Sprite::getState()
{
    return state;
}

bool Animated_Sprite::done()
{
    return (lastFrame() && frameTimer.getElapsedTime().asMilliseconds() >= frameThreshold);
}

bool Animated_Sprite::lastFrame()
{
    return animations[state][direction].lastFrame();
}

sf::Vector2f Animated_Sprite::getSize()
{
    sf::Vector2f size;
    size.x = getGlobalBounds().width;
    size.y = getGlobalBounds().height;
    return size;
}
