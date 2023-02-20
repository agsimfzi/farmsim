#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include <animation/animated_sprite.hpp>

#include <world/direction.hpp>
#include <world/vehicle.hpp>

#include "entity_data.hpp"
#include "entity_state.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Entity : public sf::Drawable {
public:
    Entity() = default;
    Entity(Entity_Data& e, sf::Texture& texture);

    sf::Vector2f getPosition();

    void setState(Entity_State nstate);

    virtual void update();

    void stop();

    void setPosition(sf::Vector2f pos);

    sf::Vector2f getVelocity();

    sf::Vector2i getCoordinates(float tileSize);

    void move();
    sf::Vector2f move(std::vector<sf::FloatRect> walls, float deltaTime);
    void moveX();
    void moveY();

    void unmove();
    void unmoveX();
    void unmoveY();

    Animated_Sprite<Entity_State>& getSprite();

    Entity_State getState();

    const std::string& getName() const;
    const std::string& getDescription() const;

    std::shared_ptr<Vehicle> getVehicle();
    Vehicle::Type getVehicleType();
    void setVehicle(std::shared_ptr<Vehicle> v);

protected:
    Entity_State state = Entity_State::IDLE;
    Entity_State lastState = Entity_State::IDLE;

    Animated_Sprite<Entity_State> sprite;

    std::string name;
    std::string description;

    void animate();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    float speed_orthogonal;
    float speed_diagonal;

    void move(sf::Vector2f v);

    sf::Vector2f velocity;

    void setVelocity();

    void setSpriteDirection();

    void directCheck();

    sf::FloatRect bounds;

    void placeBounds();

    unsigned int bounds_y_offset;

    bool up = false,
         down = false,
         left = false,
         right = false;

    std::shared_ptr<Vehicle> vehicle;

    float base_speed;

    bool movingVehicle();
};
