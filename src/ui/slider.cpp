#include <ui/slider.hpp>

#include <audio/sound_bus.hpp>

#include <util/fmouse.hpp>

//////////////////////////////////////////////////////////////

const sf::Vector2f Slider::containerSize = sf::Vector2f(384.f, 48.f);

const float Slider::offset = 3.f;

const float Slider::scrollFactor = 5.f;

Slider::Slider(std::string ntitle)
{
    container.setSize(containerSize);
    container.setFillColor(sf::Color::Transparent);
    container.setOutlineThickness(1);

    fill.setSize(sf::Vector2f(0.f, container.getSize().y - (offset * 2.f)));
    fill.setFillColor(palette::orange);

    setFill(100.f);

    title.setString(ntitle);
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(title, states);
    target.draw(container, states);
    target.draw(fill, states);
    target.draw(label, states);
}

void Slider::set(sf::Vector2f pos, sf::Font& font)
{
    title.setFont(font);

    label.setFont(font);

    title.setPosition(pos);

    pos.y += title.getLocalBounds().height + title.getLocalBounds().top + (offset * 2.f);

    container.setPosition(pos);
    fill.setPosition(pos + sf::Vector2f(offset, offset));

    pos.x += container.getSize().x - (label.getLocalBounds().width + label.getLocalBounds().left) - (offset * 4.f);
    pos.y += (fill.getSize().y - (label.getLocalBounds().height + label.getLocalBounds().top)) / 2.f - offset;
    label.setPosition(pos);

    title.setFillColor(palette::white);
    label.setFillColor(palette::white);

    container.setOutlineColor(palette::white);
}

void Slider::setFill(float f)
{
    int lastFill = static_cast<int>(getFill());
    if (f < 0.f) {
        f = 0.f;
    }
    else if (f > 100.f) {
        f = 100.f;
    }
    int newFill = static_cast<int>(f);

    if (newFill != lastFill) {
        Sound_Bus::addSound(Sound_UI::MOVE_SLIDER);
    }

    fill.setSize(sf::Vector2f((f / 100.f) * (container.getSize().x - (offset * 2.f)), fill.getSize().y));
    label.setString(std::to_string(newFill));
}

void Slider::findFill(int mX)
{
    static float f;
    if (mX <= container.getPosition().x + offset) {
        f = 0.f;
    }
    else if (mX >= container.getPosition().x + container.getSize().x - offset) {
        f = 1.f;
    }
    else {
        f = (mX - container.getPosition().x) / container.getSize().x;
    }

    f *= 100.f;

    setFill(f);
}

float Slider::getFill() const
{
    return 100.f * (fill.getSize().x / (container.getSize().x - (offset * 2.f)));
}

void Slider::scroll(float delta)
{
    if (delta < 0) {
        setFill(getFill() - scrollFactor);
    }
    else if (delta > 0) {
        setFill(getFill() + scrollFactor);
    }
}

bool Slider::checkMouse()
{
    return container.getGlobalBounds().contains(fMouse());
}

void Slider::update()
{
    if (changing) {
        findFill(sf::Mouse::getPosition().x);
    }
}

bool Slider::unclick()
{
    if (changing) {
        changing = false;
        return true;
    }
    return false;
}

bool Slider::click()
{
    if (!changing && checkMouse()) {
        changing = true;
    }
    return changing;
}
