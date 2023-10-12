#include <ui/dialogue.hpp>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>

Dialogue::Dialogue()
{
    text.setFont(Font_Manager::get(Font::UI));

    frame.setFillColor(Palette::inventory_bg);
    frame.setOutlineThickness(outline_thickness);
    frame.setSize(sf::Vector2f(1024.f, 392.f));
}

void Dialogue::setPosition(const sf::Vector2f& pos)
{
    frame.setPosition(pos);
    text.setPosition(pos);
    sprite.setPosition(pos);
}

const sf::Vector2f& Dialogue::frameSize()
{
    return frame.getSize();
}

void Dialogue::load(NPC* npc)
{
    this->npc = npc;
    conversation = npc->getConversation();
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    target.draw(sprite, states);
    target.draw(text, states);
}
