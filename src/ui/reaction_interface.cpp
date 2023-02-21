#include <ui/reaction_interface.hpp>

#include <cassert>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>


#include <iostream>

const float Reaction_Panel::outline_thickness = 1.f;

Reaction_Panel::Reaction_Panel(Reaction& rxn, Item_Library& item_library, sf::Vector2f pos, float size_x)
{
    sf::Vector2f frame_size(size_x, 66.f);
    frame.setSize(frame_size);
    frame.setOutlineThickness(outline_thickness);
    frame.setOutlineColor(Palette::inventory_outline);
    frame.setPosition(pos);

    product = item_library.shared(rxn.product);
    sf::Vector2f product_pos(pos);
    product_pos.x += size_x;
    product_pos += sf::Vector2f(-33.f, 33.f);
    product->setPosition(product_pos);

    pos += sf::Vector2f(4.f, 4.f);
    name.setPosition(pos);
    name.setString(rxn.product);
    name.setFont(Font_Manager::get(Font::UI));
    name.setFillColor(Palette::black);
    name.setCharacterSize(32);

    for (auto& r : rxn.reagants) {
        reagants.push_back(item_library.shared(r.name));
    }

    if (product) {
        std::cout << "\tloaded product from reaction " << rxn.name << ": " << product->getName() << "!\n";
    }
    else {
        std::cout << "\tFAILED TO LOAD PRODUCT FROM REACTION " << rxn.name << ": " << product->getName() << "!\n";
    }

    unsetAvailable();

    tooltip = std::make_shared<Tooltip>(rxn, reagants);
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
    frame.setFillColor(Palette::inventory_bg);
}

void Reaction_Panel::unsetAvailable()
{
    available = false;
    frame.setFillColor(Palette::inventory_bg_unavailable);
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
    target.draw(name, states);
    target.draw(*product, states);
}

///////////////////////////////////////////////////////////////////////////////////////////

void Reaction_Interface::load(std::vector<Reaction> rxn, Player_Inventory& inventory, Item_Library& item_library)
{
    reactions = rxn;

    panels.clear();

    float size_x = view.getSize().x - 20.f;

    sf::Vector2f pos(4.f, 4.f);

    for (auto& r : reactions) {
        panels.push_back(Reaction_Panel(r, item_library, pos, size_x));
        pos.y += 74.f;
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
            //r = &reactions[i];
            //p = panels[i].getProduct();
            return std::make_pair(&reactions[i], panels[i].getProduct());
        }
    }
    return std::make_pair(nullptr, nullptr);
    return std::make_pair<Reaction*, std::shared_ptr<Item>>(nullptr, nullptr);
    return std::make_pair(r, p);
}

std::shared_ptr<Tooltip> Reaction_Interface::findTooltip(sf::Vector2f mpos)
{

    size_t n = reactions.size();
    assert(n == panels.size());
    for (size_t i = 0; i < n; i++) {
        if (panels[i].contains(mpos)) {
            return panels[i].tooltip;
        }
    }
    return nullptr;
}

void Reaction_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.setView(view);
    for (const auto& p : panels) {
        target.draw(p, states);
    }

    target.draw(scrollbar, states);
}
