#include <entity/player.hpp>

#include <item/item_library.hpp>

//////////////////////////////////////////////////////////////

Player::Player() { }

Player::Player(Entity_Data e, sf::Texture& texture)
    : Entity(e, texture)
{}

void Player::tick()
{
    energy_add_index++;
    if (energy_add_index >= energy_add_threshold) {
        energy_add_index = 0;
        energy += 100;
        if (energy > max_energy) {
            energy = max_energy;
        }
    }
}

void Player::update()
{
    Entity::update();
}

void Player::upStart()
{
    if (!up) {
        up = true;
        setVelocity();
    }
}

void Player::upEnd()
{
    if (up) {
        up = false;
        setVelocity();
    }
}

void Player::downStart()
{
    if (!down) {
        down = true;
        setVelocity();
    }
}

void Player::downEnd()
{
    if (down) {
        down = false;
        setVelocity();
    }
}

void Player::leftStart()
{
    if (!left) {
        left = true;
        setVelocity();
    }
}

void Player::leftEnd()
{
    if (left) {
        left = false;
        setVelocity();
    }
}

void Player::rightStart()
{
    if (!right) {
        right = true;
        setVelocity();
    }
}

void Player::rightEnd()
{
    if (right) {
        right = false;
        setVelocity();
    }
}
