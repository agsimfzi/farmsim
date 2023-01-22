#include <entity/entity.hpp>

#include <audio/sound_bus.hpp>

#include <util/primordial.hpp>
#include <util/prng.hpp>

#define SQRT2_INV 0.707106781

//////////////////////////////////////////////////////////////

Entity::Entity(Entity_Data& data, sf::Texture& texture)
{
    name = data.name;
    description = data.description;

    velocity = sf::Vector2f(0.f, 0.f);
    speed_orthogonal = data.speed;
    speed_diagonal = speed_orthogonal * SQRT2_INV;

    name = data.name;
    sf::Vector2i size = data.size;
    sprite = Animated_Sprite(texture, size, data.aCount, data.aThreshold);
}

sf::Vector2f Entity::getPosition()
{
    return sprite.getPosition();
}

void Entity::update()
{
    sprite.update();
}

void Entity::move()
{
    if (state == Entity_State::MOVING) {
        move(velocity);
    }
}

sf::Vector2f Entity::move(std::vector<sf::FloatRect> walls, float deltaTime)
{
    sf::Vector2f offset(0.f, 0.f);
    if (offset == velocity) {
        return offset;
    }

    sf::Vector2f v = velocity * deltaTime;

    if (state == Entity_State::MOVING) {
        bool good = true;
        sprite.move(v.x, 0.f);
        for (const auto& wall : walls) {
            if (wall.intersects(sprite.getGlobalBounds())) {
                sprite.move(-v.x, 0.f);
                good = false;
            }
        }

        if (good) {
            offset.x = v.x;
        }
        else {
            good = true; //prep good for y movement
        }

        sprite.move(0.f, v.y);
        for (const auto& wall : walls) {
            if (wall.intersects(sprite.getGlobalBounds())) {
                sprite.move(0.f, -v.y);
                good = false;
            }
        }
        if (good) {
            offset.y = v.y;
        }
    }

    return offset;
}

void Entity::moveX()
{
    move(sf::Vector2f(velocity.x, 0.f));
}

void Entity::moveY()
{
    move(sf::Vector2f(0.f, velocity.y));
}

void Entity::unmove()
{
    move(-velocity);
}

void Entity::unmoveX()
{
    move(sf::Vector2f(-velocity.x, 0.f));
}

void Entity::unmoveY()
{
    move(sf::Vector2f(0.f, -velocity.y));
}

void Entity::move(sf::Vector2f v)
{
    sprite.move(v);
}

Animated_Sprite& Entity::getSprite()
{
    return sprite;
}

void Entity::stop()
{
    up = false;
    down = false;
    left = false;
    right = false;
    setVelocity();
    setState(Entity_State::IDLE);
}

void Entity::setPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

sf::Vector2f Entity::getVelocity()
{
    return velocity;
}

sf::Vector2i Entity::getCoordinates(float tileSize)
{
    sf::Vector2f pos = getPosition();
    pos += sf::Vector2f(sign(pos.x) * (tileSize / 2.f), sign(pos.y) * (tileSize / 2.f));
    sf::Vector2i c;
    c.x = pos.x / tileSize;
    c.y = pos.y / tileSize;
    return c;
}

void Entity::setSpriteDirection()
{
    Direction d = sprite.getDirection();
    float threshold = speed_orthogonal * 0.15f;

    bool n = (velocity.y < -threshold),
         e = (velocity.x > threshold),
         s = (velocity.y > threshold),
         w = (velocity.x < -threshold);

    if (n) {
        if (e) {
            d = Direction::NE;
        }
        else if (w) {
            d = Direction::NW;
        }
        else {
            d = Direction::N;
        }
    }
    else if (s) {
        if (e) {
            d = Direction::SE;
        }
        else if (w) {
            d = Direction::SW;
        }
        else {
            d = Direction::S;
        }
    }
    else {
        if (e) {
            d = Direction::E;
        }
        else if (w) {
            d = Direction::W;
        }
    }

    sprite.setDirection(d);
}

void Entity::setVelocity()
{
    float speed = speed_orthogonal;
    if ((up && left) || (up && right) || (down && left) || (down && right)) {
        speed = speed_diagonal;
    }

    if ((!left && !right) || (left && right)) {
        velocity.x = 0.f;
    }
    else if (left && !right) {
        velocity.x = -speed;
    }
    else if (!left && right) {
        velocity.x = speed;
    }

    if (!up && !down) {
        velocity.y = 0.f;
    }
    else if (up && !down) {
        velocity.y = -speed;
    }
    else if (!up && down) {
        velocity.y = speed;
    }

    if (velocity.x == 0.f && velocity.y == 0.f) {
        setState(Entity_State::IDLE);
    }
    else if (sprite.getAnimationState() != Entity_State::MOVING
        && (velocity.x != 0.f || velocity.y != 0.f)) {
        setState(Entity_State::MOVING);
        setSpriteDirection();
    }
    else {
        setSpriteDirection();
    }
}

void Entity::setState(Entity_State nstate)
{
    if (state != nstate) {
        lastState = state;
        state = nstate;
        sprite.setAnimationState(state);
    }
}

Entity_State Entity::getState()
{
    return state;
}

const std::string& Entity::getName() const
{
    return name;
}

const std::string& Entity::getDescription() const
{
    return description;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
