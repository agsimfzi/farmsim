#include <ui/player_target.hpp>

#include <world/tile.hpp>

Player_Target::Player_Target()
{
    float stroke_width = 4.f;
    frame.setSize(sf::Vector2f(Tile::tile_size - stroke_width, Tile::tile_size - stroke_width));
    frame.setOrigin(frame.getSize() / 2.f);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(stroke_width);
    frame.setOutlineColor(sf::Color(200, 50, 50));
}

void Player_Target::set(const sf::Vector2i& coords)
{
    active = true;
    frame.setPosition(sf::Vector2f(coords));
}

void Player_Target::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (active) {
        target.draw(frame, states);
    }
}
