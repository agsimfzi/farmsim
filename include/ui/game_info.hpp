#pragma once

#include <SFML/Graphics.hpp>

#include "energy_bar.hpp"
#include "wallet_inspector.hpp"

class Game_Info : public sf::Drawable {
public:
    Game_Info() = default;

    Wallet_Inspector wallet;

    sf::RectangleShape season;

    Energy_Bar energy;

    int last_player_energy{ -1 };

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
