#include <ui/building_interface.hpp>

Building_Interface::Building_Interface()
{
    frame.setPosition(sf::Vector2f(1300.f, 200.f));
    frame.setSize(sf::Vector2f(512.f, 512.f));
    frame.setOutlineThickness(1.f);
    frame.setOutlineColor(sf::Color::Black);
    frame.setFillColor(sf::Color(177, 181, 171));
}

void Building_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    target.draw(title, states);
    target.draw(reagant, states);
    target.draw(product, states);
}
