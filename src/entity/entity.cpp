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
    base_speed = data.speed;
    speed_orthogonal = data.speed;
    speed_diagonal = speed_orthogonal * SQRT2_INV;

    name = data.name;
    sf::Vector2i size = data.size;
    sprite = Animated_Sprite(texture, size, data.aCount, data.aThreshold);

    bounds_y_offset = data.bounds_y_offset;

    bounds.width = data.bounds_x_size;
    bounds.height = data.bounds_y_size;
}

sf::Vector2f Entity::getPosition()
{
    return sprite.getPosition();
}

void Entity::update()
{
    sprite.update();
    if (vehicle) {
        vehicle->update();
    }
}

void Entity::move()
{
    if (state == Entity_State::MOVING || movingVehicle()) {
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

    if (state == Entity_State::MOVING || movingVehicle()) {
        bool good = true;
        sprite.move(v.x, 0.f);
        placeBounds();
        for (const auto& wall : walls) {
            if (wall.intersects(bounds)) {
                sprite.move(-v.x, 0.f);
                placeBounds();
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
        placeBounds();
        for (const auto& wall : walls) {
            if (wall.intersects(bounds)) {
                sprite.move(0.f, -v.y);
            placeBounds();
                good = false;
            }
        }
        if (good) {
            offset.y = v.y;
        }

        if (vehicle) {
            vehicle->move(offset);
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

Animated_Sprite<Entity_State>& Entity::getSprite()
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
    if (vehicle) {
        vehicle->setDirection(d);
    }
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
    else if ((sprite.getState() != Entity_State::MOVING || movingVehicle())
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
    if (vehicle) {
        Vehicle_State v = Vehicle_State::IDLE;
        if (nstate == Entity_State::MOVING) {
            v = Vehicle_State::MOVING;
        }
        vehicle->setAnimationState(v);
    }
    else if (state != nstate) {
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

std::shared_ptr<Vehicle> Entity::getVehicle()
{
    return vehicle;
}

Vehicle::Type Entity::getVehicleType()
{
    if (!vehicle) {
        return Vehicle::NULL_TYPE;
    }
    return vehicle->type;
}

void Entity::setVehicle(std::shared_ptr<Vehicle> v)
{
    float speed_factor = 1.f;
    if (v) {
        speed_factor = v->speed_factor;
                setPosition(v->getPosition());
        switch (v->type) {
            // will also need to modify the bounding box, and probably other little stuff.
            default:
                break;
            case Vehicle::BOAT:
                setState(Entity_State::BOATING);
                break;
            case Vehicle::BROOM:
                setState(Entity_State::BROOMING);
                break;
        }
    }
    else {
        setState(Entity_State::IDLE);
    }
    vehicle = v;
    speed_orthogonal = base_speed * speed_factor;
    speed_diagonal = speed_orthogonal * SQRT2_INV;
}

void Entity::placeBounds()
{
    sf::Vector2f pos = getPosition();
    pos -= sprite.getOrigin();
    pos.x += (sprite.getGlobalBounds().width - bounds.width) / 2;
    pos.y += (sprite.getGlobalBounds().height - bounds.height - bounds_y_offset);

    bounds.left = pos.x;
    bounds.top = pos.y;
}

bool Entity::movingVehicle()
{
    return (vehicle && vehicle->getState() == Vehicle_State::MOVING);
}

sf::FloatRect Entity::getBounds()
{
    return bounds;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (vehicle) {
        target.draw(*vehicle, states);
    }
    target.draw(sprite, states);
}
