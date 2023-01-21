#include <ui/button.hpp>

#include <resources/palette.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>

//////////////////////////////////////////////////////////////

const float Button::padding = 8;

Button::Button(std::string nlabel, sf::Font& font)
{
    label.setFont(font);
    label.setString(nlabel);
    label.setCharacterSize(72);
    centerText(label);
    container.setSize(sf::Vector2f(label.getLocalBounds().left + label.getLocalBounds().width + (padding * 2.f),
        label.getLocalBounds().top + label.getLocalBounds().height + (padding * 2.f)));
    container.setOrigin(container.getSize() / 2.f);
    unhighlight();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(container, states);
    target.draw(label, states);
}

void Button::update(sf::Vector2f& mpos)
{
    bool contains = container.getGlobalBounds().contains(mpos);
    if (!highlighted && contains) {
        highlight();
    }
    else if (highlighted && !contains) {
        unhighlight();
    }
}

bool Button::isHighlighted()
{
    return highlighted;
}

void Button::highlight()
{
    highlighted = true;
    container.setFillColor(palette::orange);
    label.setFillColor(palette::purple);
}

void Button::unhighlight()
{
    highlighted = false;
    label.setFillColor(palette::orange);
    container.setFillColor(palette::purple);
}

void Button::setPosition(sf::Vector2f pos)
{
    label.setPosition(pos);
    container.setPosition(pos);
}

sf::Vector2f Button::getPosition()
{
    return label.getPosition();
}
