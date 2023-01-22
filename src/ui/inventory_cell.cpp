#include <ui/inventory_cell.hpp>

#include <resources/font_manager.hpp>

const sf::Vector2f Inventory_Cell::frameSize = sf::Vector2f(64.f, 64.f);
const float Inventory_Cell::frameOutlineSize = 2.f;

const sf::Color Inventory_Cell::colorActive = sf::Color(252, 244, 212);
const sf::Color Inventory_Cell::colorInactive = sf::Color(230, 209, 135);
const sf::Color Inventory_Cell::colorOutline = sf::Color(25, 25, 25);

Inventory_Cell::Inventory_Cell(Item* item)
{
    setItem(item);
    frame.setOutlineColor(colorOutline);
    frame.setOutlineThickness(frameOutlineSize);
    frame.setSize(frameSize);
    frame.setOrigin(frameSize / 2.f);
    deactivate();
    numberText.setFont(Font_Manager::get(Font::UI));
    numberText.setFillColor(sf::Color::Black);
}

void Inventory_Cell::setPosition(sf::Vector2f pos)
{
    frame.setPosition(pos);
    if (item) {
        item->setPosition(pos);
    }
    pos.x -= frame.getSize().x * .4f;
    numberText.setPosition(pos);
}

void Inventory_Cell::activate()
{
    active = true;
    frame.setFillColor(colorActive);
}

void Inventory_Cell::deactivate()
{
    active = false;
    frame.setFillColor(colorInactive);
}

void Inventory_Cell::setItem(Item* i)
{
    if (i) {
        if (!item || i->getUID() != item->getUID()) {
            item = std::make_shared<Item>(*i);
            item->setPosition(frame.getPosition());
        }

        size_t count = i->count();

        if (i->count() == 1) {
            numberText.setString("");
        }
        else if (i->count()) {
            numberText.setString(std::to_string(count));
        }
    }
    else if (item) {
        item = nullptr;
        numberText.setString("");
    }
}

void Inventory_Cell::clearItem()
{
    item = nullptr;
}

Item* Inventory_Cell::getItem()
{
    return item.get();
}

void Inventory_Cell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    if (item) {
        target.draw(*item, states);
        target.draw(numberText, states);
    }
}
