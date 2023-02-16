#include <ui/reaction_interface.hpp>

#include <cassert>

#include <resources/font_manager.hpp>

#include <iostream>

const sf::Color Reaction_Panel::color_bg_available = sf::Color(230, 209, 135);
const sf::Color Reaction_Panel::color_bg_unavailable = sf::Color(82, 82, 82);
const float Reaction_Panel::outline_thickness = 1.f;

Reaction_Panel::Reaction_Panel(Reaction& rxn, Item_Library& item_library, sf::Vector2f pos)
{
    frame.setPosition(pos);
    frame.setSize(sf::Vector2f(392.f, 128.f));
    frame.setOutlineThickness(outline_thickness);
    frame.setOutlineColor(sf::Color::Black);

    pos.x += 48.f;
    pos.y += 80.f;

    for (auto& r : rxn.reagants) {
        reagants.push_back(item_library.shared(r.name));
        reagants.back()->setPosition(pos);

        pos.x += 96.f;
    }

    // place arrow

    pos.x += 96.f;

    product = item_library.shared(rxn.product);
    product->setPosition(pos);

    if (product) {
        std::cout << "\tloaded product from reaction " << rxn.name << ": " << product->getName() << "!\n";
    }
    else {
        std::cout << "\tFAILED TO LOAD PRODUCT FROM REACTION " << rxn.name << ": " << product->getName() << "!\n";
    }

    unsetAvailable();
}

Reaction_Panel::~Reaction_Panel()
{
    if (product) {
        product.reset();
    }
}

bool Reaction_Panel::isAvailable()
{
    return available;
}

void Reaction_Panel::setAvailable()
{
    available = true;
    frame.setFillColor(color_bg_available);
}

void Reaction_Panel::unsetAvailable()
{
    available = false;
    frame.setFillColor(color_bg_unavailable);
}

bool Reaction_Panel::contains(sf::Vector2f mpos)
{
    return frame.getGlobalBounds().contains(mpos);
}

std::shared_ptr<Item> Reaction_Panel::getProduct()
{
    return product;
}

void Reaction_Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    for (const auto& r : reagants) {
        target.draw(*r, states);
    }
    target.draw(*product, states);
}

///////////////////////////////////////////////////////////////////////////////////////////

void Reaction_Interface::load(std::vector<Reaction> rxn, Player_Inventory& inventory, Item_Library& item_library)
{
    std::cout << "LOADING REACTION INTERFACE...\n";
    reactions = rxn;

    panels.clear();

    sf::Vector2f pos(4.f, 4.f);

    for (auto& r : reactions) {
        panels.push_back(Reaction_Panel(r, item_library, pos));
        pos.y += 132.f;
    }
    setScrollable(pos.y);
    check(inventory);
}

void Reaction_Interface::check(Player_Inventory& inventory)
{
    size_t n = reactions.size();
    for (size_t i = 0; i < n; i++) {
        Reaction& r = reactions[i];
        Reaction_Panel& p = panels[i];
        bool available = true;

        for (auto& reagant : r.reagants) {
            if (inventory.countItem(reagant.name) < reagant.count) {
                available = false;
            }
        }

        if (available) {
            p.setAvailable();
        }
        else {
            p.unsetAvailable();
        }
    }
}

void Reaction_Interface::close()
{
    reactions.clear();
    panels.clear();
    reset();
}

std::pair<Reaction*, std::shared_ptr<Item>> Reaction_Interface::click(sf::Vector2f mpos)
{
    Reaction* r = nullptr;
    std::shared_ptr<Item> p = nullptr;
    size_t n = reactions.size();
    assert(n == panels.size());
    for (size_t i = 0; i < n; i++) {
        if (panels[i].contains(mpos) && panels[i].isAvailable()) {
            r = &reactions[i];
            p = panels[i].getProduct();
        }
    }
    return std::make_pair(r, p);
}

void Reaction_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.setView(view);
    for (const auto& p : panels) {
        target.draw(p, states);
    }

    target.draw(scrollbar, states);
}
