#include <ui/inventory_cell.hpp>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>

const float Inventory_Cell::size = 64.f;
const float Inventory_Cell::frame_thickness = 2.f;
const sf::Color Inventory_Cell::colorUseBar = sf::Color(40, 250, 60);

const sf::Vector2f Inventory_Cell::use_bar_size = sf::Vector2f(64.f, 6.f);

Inventory_Cell::Inventory_Cell(std::shared_ptr<Item> i)
{
    setItem(i);
    frame.setOutlineColor(Palette::inventory_outline);
    frame.setOutlineThickness(frame_thickness);
    frame.setSize(sf::Vector2f(size, size));
    frame.setOrigin(size / 2.f, size / 2.f);
    deactivate();
    count_text.setFont(Font_Manager::get(Font::UI));
    count_text.setFillColor(Palette::black);
    count_text.setOutlineColor(Palette::white);
    count_text.setOutlineThickness(2.f);
    count_text.setCharacterSize(character_size);

    use_bar.setFillColor(colorUseBar);
}

void Inventory_Cell::setPosition(sf::Vector2f pos)
{
    frame.setPosition(pos);
    if (item) {
        item->setPosition(pos);
    }
    pos.x -= frame.getSize().x * .45f;
    pos.y += frame.getSize().y / 2.f;
    pos.y -= (character_size + 2);
    count_text.setPosition(pos);

    pos = frame.getPosition() - frame.getOrigin();
    pos.y += size - use_bar_size.y;
    use_bar.setPosition(pos);
}

void Inventory_Cell::activate()
{
    frame.setFillColor(Palette::inventory_bg_active);
}

void Inventory_Cell::deactivate()
{
    frame.setFillColor(Palette::inventory_bg);
}

void Inventory_Cell::setItem(std::shared_ptr<Item> i)
{
    if (i) {
        if (!item) {
            item = i;
            item->setPosition(frame.getPosition());
            sf::Vector2f origin;
            origin.x = item->getSprite().getGlobalBounds().width / 2.f;
            origin.y = item->getSprite().getGlobalBounds().height / 2.f;
            item->getSprite().setOrigin(origin);
        }
        else if (item->getUID() == i->getUID()) {
            item->setCount(i->count());
        }

        std::string number = "";

        if (item->count() > 1) {
            number = std::to_string(item->count());
        }
        count_text.setString(number);

        usable = false;

        if (item->getUID() == 1) { // watering can
            usable = true;
            calculateUseBarSize(item->usePercent());
        }
    }
    else if (item) {
        clearItem();
    }
}

void Inventory_Cell::calculateUseBarSize(int percent)
{
    sf::Vector2f bsize = use_bar_size;
    bsize.x *= (percent / 100.f);
    use_bar.setSize(bsize);
}

void Inventory_Cell::clearItem()
{
    item = nullptr;
    count_text.setString("");
}

std::shared_ptr<Item> Inventory_Cell::getItem()
{
    return item;
}

void Inventory_Cell::add(size_t count)
{
    if (item) {
        item->add(count);
        updateCount();
    }
}

void Inventory_Cell::take(size_t count)
{
    if (item) {
        item->take(count);
        updateCount();
    }
}

void Inventory_Cell::updateCount()
{
    if (item) {
        if (item->count() == 1) {
            count_text.setString("");
        }
        else if (item->count() == 0) {
            clearItem();
        }
        else {
            count_text.setString(std::to_string(item->count()));
        }
    }
}

void Inventory_Cell::setCount(size_t count)
{
    item->setCount(count);
}

sf::Vector2f Inventory_Cell::getPosition()
{
    return frame.getPosition();
}

void Inventory_Cell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    if (item) {
        target.draw(*item, states);
        target.draw(count_text, states);
        if (usable) {
            target.draw(use_bar, states);
        }
    }
}
