#include <ui/button.hpp>

#include <audio/sound_bus.hpp>
#include <audio/sound.hpp>

#include <resources/palette.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>

//////////////////////////////////////////////////////////////

const float Button::padding = 8;

Button::Button(std::string nlabel, sf::Font& font, unsigned int csize)
{
    label.setFont(font);
    label.setString(nlabel);
    label.setCharacterSize(csize);
    centerText(label);
    frame.setSize(sf::Vector2f(label.getLocalBounds().left + label.getLocalBounds().width + (padding * 2.f),
        label.getLocalBounds().top + label.getLocalBounds().height + (padding * 2.f)));
    frame.setOrigin(frame.getSize() / 2.f);
    unhighlight();
    setAvailable();
}

Button::Button(std::string nlabel, sf::Font& font, std::function<void()> target, unsigned int csize)
    : target{ target }
{
    label.setFont(font);
    label.setString(nlabel);
    label.setCharacterSize(72);
    centerText(label);
    frame.setSize(sf::Vector2f(label.getLocalBounds().left + label.getLocalBounds().width + (padding * 2.f),
        label.getLocalBounds().top + label.getLocalBounds().height + (padding * 2.f)));
    frame.setOrigin(frame.getSize() / 2.f);
    unhighlight();
    setAvailable();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    target.draw(label, states);
}

void Button::update(sf::Vector2f& mpos)
{
    bool contains = frame.getGlobalBounds().contains(mpos);
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
    frame.setFillColor(Palette::green);
    label.setFillColor(Palette::white);
}

void Button::unhighlight()
{
    highlighted = false;
    label.setFillColor(Palette::white);
    frame.setFillColor(Palette::gray);
}

void Button::setPosition(sf::Vector2f pos)
{
    label.setPosition(pos);
    frame.setPosition(pos);
}

sf::Vector2f Button::getPosition()
{
    return label.getPosition();
}

sf::Vector2f Button::getSize()
{
    return frame.getSize();
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
    frame.setFillColor(Palette::gray_dark);
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

Nav::Nav(std::string nlabel, sf::Font& font, Main_State ntmain, Menu_State ntmenu, unsigned int csize)
    : Button { nlabel, font, csize }
    , target_main{ ntmain }
    , target_menu{ ntmenu }
{}
