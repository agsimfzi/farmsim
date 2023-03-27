#include <ui/wallet_inspector.hpp>

#include <resources/font_manager.hpp>

#include <resources/palette.hpp>

Wallet_Inspector::Wallet_Inspector()
{
    frame.setSize(sf::Vector2f(256.f, 72.f));
    frame.setOrigin(sf::Vector2f(frame.getSize().x, frame.getSize().y / 2.f));
    frame.setPosition(sf::Vector2f(1904.f, 560.f));
    frame.setFillColor(Palette::inventory_bg);
    frame.setOutlineColor(Palette::inventory_outline);
    frame.setOutlineThickness(1.f);

    icon.setOrigin(sf::Vector2f(-16.f, 32.f));
    // icon.setTexture(Texture_Manager::get()
    // icon.setTextureRect()
    icon.setPosition(frame.getPosition() - sf::Vector2f(frame.getSize().x, 0.f));

    text.setFont(Font_Manager::get(Font::UI));
    text.setFillColor(Palette::black);
    update(0);
}

void Wallet_Inspector::update(int count)
{
    text.setString(std::to_string(count));

    sf::FloatRect trect = text.getLocalBounds();
    sf::Vector2f origin;
    origin.x = trect.left + trect.width + 16.f;
    origin.y = trect.top + trect.height / 2.f;
    text.setOrigin(origin);
    text.setPosition(frame.getPosition());
}

void Wallet_Inspector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    target.draw(icon, states);
    target.draw(text, states);
}
