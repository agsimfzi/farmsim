#include <ui/game_info.hpp>


void Game_Info::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(wallet, states);
    target.draw(season, states);
    target.draw(energy, states);
}
