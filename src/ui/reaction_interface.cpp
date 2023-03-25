#include <ui/reaction_interface.hpp>

#include <cassert>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>

const float Reaction_Panel::outline_thickness = 1.f;

Reaction_Panel::Reaction_Panel(Reaction& rxn, Library& library, sf::Vector2f pos)
{
    sf::Vector2f frame_size(66.f, 66.f);
    frame.setSize(frame_size);
    frame.setOutlineThickness(outline_thickness);
    frame.setOutlineColor(Palette::inventory_outline);
    frame.setPosition(pos);

    product = library.item(rxn.product);
    sf::Vector2f product_pos(pos);
    product_pos += (frame_size / 2.f);
    product->setPosition(product_pos);

    for (auto& r : rxn.reagants) {
        reagants.push_back(library.item(r.name));
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
    target.draw(*product, states);
}

///////////////////////////////////////////////////////////////////////////////////////////

void Reaction_Interface::load(std::vector<Reaction> rxn
                            , Player_Inventory& inventory
                            , Library& library
                            , sf::Vector2f pos)
{
    scrollbar.setSize(sf::Vector2f(12.f, 12.f));
    const static float max_size = 390.f;
    reactions = rxn;

    panels.clear();

    sf::Vector2f c_pos(4.f, 4.f);

    float sx = c_pos.x;

    std::string tag = rxn.front().tag;

    sf::Vector2f size(0.f, 0.f);

    float full_size_y = 0.f;

    for (auto& r : reactions) {
        if (r.tag != tag) {
            tag = r.tag;
            c_pos.x = sx;
            c_pos.y += 74.f;
        }
        panels.push_back(Reaction_Panel(r, library, c_pos));
        if (c_pos.x + 70.f > size.x) {
            size.x = c_pos.x + 70.f;
        }
        if (c_pos.y + 70.f > size.y) {
            size.y = c_pos.y + 70.f;
            full_size_y = size.y;
            if (size.y > max_size) {
                size.y = max_size;
            }
        }
        c_pos.x += 74.f;
    }

    size.x += scrollbar.getSize().x;
    setView(pos, size);
    setScrollable(full_size_y);

    size.x -= 4.f;
    size.y = full_size_y - 2.f;
    frame.setPosition(sf::Vector2f(1.f, 1.f));
    frame.setSize(size);
    frame.setFillColor(Palette::inventory_bg);
    frame.setOutlineColor(Palette::inventory_outline);
    frame.setOutlineThickness(1.f);
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
            break;
        }
    }
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
    target.draw(frame, states);
    for (const auto& p : panels) {
        target.draw(p, states);
    }

    target.draw(scrollbar, states);
}
