#pragma once

#include <game/wallet.hpp>

#include <world/direction.hpp>

#include "entity.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Player : public Entity {
public:
    Player();
    Player(Entity_Data e, sf::Texture& texture);

    void update() override;

    void tick();

    int getEnergy();
    int maxEnergy();
    void addEnergy(size_t factor);
    void takeEnergy(size_t factor);
    void setMaxEnergy();

    Direction hMove;
    Direction vMove;

    void upStart();
    void upEnd();

    void downStart();
    void downEnd();

    void leftStart();
    void leftEnd();

    void rightStart();
    void rightEnd();

    Wallet& getWallet();

private:
    unsigned int equipped = 0;

    int energy{ 1000 };
    int max_energy{ 1000 };

    size_t energy_add_index = 0;
    size_t energy_add_threshold = 15;
    size_t energy_threshold_factor = 1;
    size_t energy_restore = 5;
    size_t energy_restore_factor = 1;

    size_t item_use_index = 0;
    size_t item_use_threshold = 4;

    sf::Clock still_timer;
    float still_timer_threshold = 3.f;

    void checkEnergyFactor();

    void resetItemUseIndex();

    Wallet wallet;
};
