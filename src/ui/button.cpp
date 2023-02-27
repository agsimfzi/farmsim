#include <ui/button.hpp>

#include <audio/sound_bus.hpp>
#include <audio/sound.hpp>

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
    setAvailable();
}

Button::Button(std::string nlabel, sf::Font& font, std::function<void()> target)
    : target{ target }
{
    label.setFont(font);
    label.setString(nlabel);
    label.setCharacterSize(72);
    centerText(label);
    container.setSize(sf::Vector2f(label.getLocalBounds().left + label.getLocalBounds().width + (padding * 2.f),
        label.getLocalBounds().top + label.getLocalBounds().height + (padding * 2.f)));
    container.setOrigin(container.getSize() / 2.f);
    unhighlight();
    setAvailable();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(container, states);
    target.draw(label, states);
}

void Button::update(sf::Vector2f& mpos)
{
    bool contains = container.getGlobalBounds().contains(mpos);
    if (available) {
        if (!highlighted && contains) {
            highlight();
        }
        else if (highlighted && !contains) {
            unhighlight();
        }
    }
}

bool Button::isHighlighted()
{
    return highlighted;
}

void Button::highlight()
{
    highlighted = true;
    container.setFillColor(Palette::green);
    label.setFillColor(Palette::white);
}

void Button::unhighlight()
{
    highlighted = false;
    label.setFillColor(Palette::white);
    container.setFillColor(Palette::gray);
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

bool Button::isAvailable()
{
    return available;
}

void Button::setAvailable()
{
    available = true;
    unhighlight();
}

void Button::unsetAvailable()
{
    available = false;
    label.setFillColor(Palette::black);
    container.setFillColor(Palette::gray_dark);
}

bool Button::click()
{
    if (highlighted) {
        unhighlight();
        target();
        Sound_Bus::addSound(Sound_UI::TRIGGER_BUTTON);
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////

Nav::Nav(std::string nlabel, sf::Font& font, Main_State ntmain, Menu_State ntmenu)
    : Button { nlabel, font }
    , target_main{ ntmain }
    , target_menu{ ntmenu }
{}
