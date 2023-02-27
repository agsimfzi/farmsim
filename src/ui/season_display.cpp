#include <ui/season_display.hpp>

Season_Display::Season_Display

void Season_Display::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    target.draw(display, states);
}
