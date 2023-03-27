#include <entity/player.hpp>

//#include <item/item_library.hpp>

//////////////////////////////////////////////////////////////

Player::Player() { }

Player::Player(Entity_Data e, sf::Texture& texture)
    : Entity(e, texture)
{}

void Player::update()
{
    Entity::update();
    checkEnergyFactor();
}

void Player::tick()
{
    energy_add_index++;
    if (energy_add_index >= energy_add_threshold) {
        energy_add_index = 0;
        item_use_index++;
        if (item_use_index >= item_use_threshold) {
            item_use_index = item_use_threshold;
            addEnergy(energy_restore * energy_restore_factor);
        }
    }
}

int Player::getEnergy()
{
    return energy;
}

int Player::maxEnergy()
{
    return max_energy;
}

void Player::addEnergy(size_t factor)
{
    energy += factor;
    if (energy > max_energy) {
        energy = max_energy;
    }
}

void Player::takeEnergy(size_t factor)
{
    energy -= factor;
    if (energy < 0) {
        energy = 0;
    }
    resetItemUseIndex();
}

void Player::setMaxEnergy()
{
    energy = max_energy;
}

void Player::checkEnergyFactor()
{
    energy_restore_factor = 1;
    if (vehicle && vehicle->type == Vehicle::BOAT) {
        energy_restore_factor = 4;
    }
    else if (!up && !down && !left && !right && still_timer.getElapsedTime().asSeconds() >= still_timer_threshold) {
        energy_restore_factor = 2;
    }
    else {
        energy_restore_factor = 1;
    }
}

void Player::upStart()
{
    if (!up) {
        up = true;
        setVelocity();
        still_timer.restart();
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
        still_timer.restart();
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
        still_timer.restart();
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
        still_timer.restart();
    }
}

void Player::rightEnd()
{
    if (right) {
        right = false;
        setVelocity();
    }
}

Wallet& Player::getWallet()
{
    return wallet;
}

void Player::resetItemUseIndex()
{
    item_use_index = 0;
}
