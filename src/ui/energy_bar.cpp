#include <ui/energy_bar.hpp>

Energy_Bar::Energy_Bar()
{
    frame.setFillColor(sf::Color(230, 209, 135));
    frame.setOutlineThickness(outline_thickness);
    frame.setOutlineColor(sf::Color::Black);
    frame.setSize(sf::Vector2f(max_size, height));
}

void Energy_Bar::update(float energy)
{
    fill.setSize(sf::Vector2f(max_size * energy, height));
    fill.setFillColor(calculateFillColor(energy));
}

void Energy_Bar::setPosition(sf::Vector2f pos)
{
    frame.setPosition(pos);
    fill.setPosition(pos);
}

sf::Color Energy_Bar::calculateFillColor(float energy)
{
    return sf::Color::Green;
}

void Energy_Bar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    target.draw(fill, states);
}
