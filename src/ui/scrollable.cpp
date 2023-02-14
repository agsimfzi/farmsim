#include <ui/scrollable.hpp>

void Scrollable::setScrollable(sf::Vector2f pos, sf::Vector2f size, sf::Vector2u window_size, float max_height)
{}

void Scrollable::scroll(float delta)
{}

sf::View& Scrollable::getView()
{
    return view;
}
