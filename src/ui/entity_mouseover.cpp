#include <ui/entity_mouseover.hpp>

#include <resources/palette.hpp>

#include <util/wrap_text.hpp>

Entity_Mouseover::Entity_Mouseover(const sf::Font& font)
{
    nameText.setFont(font);
    descriptionText.setFont(font);
}

void Entity_Mouseover::update(Entity* entity)
{
    if (entity != nullptr) {
        if (!opened
         || std::string(nameText.getString()) != entity->getName()) {
            open(entity);
        }
        sprite = entity->getSprite();
        sprite.setPosition(spritePos);
    }
    else if (opened) {
        close();
    }
}

void Entity_Mouseover::set(sf::Vector2f framePos, sf::Vector2f size)
{
    float outlineThickness = 2.f;
    size.x -= outlineThickness;

    frame.setSize(size);

    frame.setPosition(framePos);
    frame.setFillColor(palette::black);
    frame.setOutlineThickness(outlineThickness);
    frame.setOutlineColor(palette::white);

    spritePos = framePos + sf::Vector2f(64.f, 64.f);
    sprite.setPosition(spritePos);

    sf::Vector2f namePos = framePos + sf::Vector2f(128.f + 16.f, 32.f);
    nameText.setPosition(namePos);
    nameText.setFillColor(palette::white);

    sf::Vector2f descriptionPos = framePos + sf::Vector2f(16.f, 128.f + 32.f);
    descriptionText.setPosition(descriptionPos);
    descriptionText.setFillColor(palette::white);
    descriptionText.setCharacterSize(22);
}

bool Entity_Mouseover::isOpen()
{
    return opened;
}

void Entity_Mouseover::open(Entity* entity)
{
    if(entity != nullptr) {
        opened = true;
        sprite = entity->getSprite();
        sprite.setPosition(spritePos);
        nameText.setString(entity->getName());
        descriptionText.setString(entity->getDescription());
        wrapText(descriptionText, frame.getSize().x);
    }
}

void Entity_Mouseover::close()
{
    opened = false;
}

void Entity_Mouseover::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (opened) {
        target.draw(frame, states);
        target.draw(sprite, states);
        target.draw(nameText, states);
        target.draw(descriptionText, states);
    }
}
