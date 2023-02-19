#pragma once

#include <world/direction.hpp>

#include "entity.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Player : public Entity {
public:
    Player();
    Player(Entity_Data e, sf::Texture& texture);

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

    void update() override;

    int energy{ 1000 };
    int max_energy{ 1000 };

    void tick();

    void resetItemUseIndex();

private:
    unsigned int equipped = 0;

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
};
