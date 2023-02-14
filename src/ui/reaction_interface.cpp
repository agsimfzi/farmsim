#include <ui/reaction_interface.hpp>

void Reaction_Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{}

///////////////////////////////////////////////////////////////////////////////////////////

void Reaction_Interface::load(std::vector<Reaction> rxn, Player_Inventory& inventory)
{
    reactions = rxn;

    panels.clear();
    for (const auto& r : reactions) {
        panels.push_back(Reaction_Panel());
    }
}

Reaction* Reaction_Interface::click(sf::Vector2f mpos)
{
    // uhhhhh
}

void Reaction_Interface::close()
{
    reactions.clear();
    panels.clear();
}

void Reaction_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& p : panels) {
        target.draw(p, states);
    }

    target.draw(scrollbar, states);
}
