#include <world/player_target.hpp>

Player_Target::Player_Target()
{
    frame.setSize(sf::Vector2f(Tile::tileSize - 4.f, Tile::tileSize - 4.f));
    frame.setOrigin(frame.getSize() / 2.f);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(4.f);
    frame.setOutlineColor(sf::Color(200, 50, 50));
}

void Player_Target::setActive(bool active)
{
    this->active = active;
}

void Player_Target::place(sf::Vector2i coords)
{
    coordinates = coords;
    frame.setPosition(sf::Vector2f(coords) * Tile::tileSize);
}

void Player_Target::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (active) {
        target.draw(frame, states);
    }
}
