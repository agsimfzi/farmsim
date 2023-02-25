#include <item/item.hpp>

Item::Item(Item_Data d)
    : Item_Data(d)
{
    if (subtype.find("EDIBLE") != std::string::npos) {
        m_edible = true;
    }
}

void Item::setSprite(sf::Sprite sprite)
{
    this->sprite = sprite;
    sf::Vector2f size(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    sprite.setOrigin(size / 2.f);
}

sf::Vector2f Item::getPosition()
{
    return sprite.getPosition();
}

std::string Item::getDescription()
{
    return description;
}

int Item::getValue()
{
    return value;
}

void Item::setPosition(sf::Vector2f pos)
{
    sprite.setPosition(pos);
}

bool Item::sellable()
{
    return value >= 0;
}

bool Item::stackable()
{
    return stack_size > 1;
}

int Item::stackSize()
{
    return stack_size;
}

std::string Item::getName()
{
    return name;
}

size_t Item::getUID()
{
    return uid;
}

Item_Type Item::getType()
{
    return type;
}

sf::Sprite& Item::getSprite()
{
    return sprite;
}

size_t Item::add(size_t diff)
{
    size_t remainder = 0;
    if (static_cast<int>(m_count + diff) > stack_size) {
        remainder = m_count + diff - stack_size;
        m_count = stack_size;
    }
    else {
        m_count += diff;
    }
    return remainder;
}

size_t Item::take(size_t diff)
{
    size_t remainder = 0;

    if (diff >= m_count) {
        m_count = 0;
        remainder = diff - m_count;
    }
    else {
        m_count -= diff;
    }

    return remainder;
}

size_t Item::count()
{
    return m_count;
}

void Item::setCount(size_t n_count)
{
    if ((int)n_count > stack_size) {
        n_count = stack_size;
    }
    m_count = n_count;
}

bool Item::useLimit()
{
    return (use_factor > 0);
}

void Item::resetUses()
{
    use_percent = 100;
}

int Item::usePercent()
{
    return use_percent;
}

int Item::useFactor()
{
    return use_factor;
}

void Item::reduceUses()
{
    use_percent -= use_factor;
    if (use_percent < 0) {
        use_percent = 0;
    }
}

bool Item::edible()
{
    return m_edible;
}

void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
