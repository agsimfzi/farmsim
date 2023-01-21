#include <entity/player.hpp>

#include <item/item_library.hpp>

#include <iostream>

//////////////////////////////////////////////////////////////

Player::Player() { }

Player::Player(Entity_Data e, sf::Texture& texture)
    : Entity(e, texture)
{
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
