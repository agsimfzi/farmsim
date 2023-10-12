#pragma once

#include <SFML/Graphics.hpp>

#include <entity/npc.hpp>

class Dialogue : public sf::Drawable {
public:
    Dialogue();

    void setPosition(const sf::Vector2f& pos);
    const sf::Vector2f& frameSize();

    void load(NPC* npc);

private:
    sf::RectangleShape frame;
    sf::Text text;
    sf::Sprite sprite;

    NPC* npc;
    Conversation conversation;

    const float outline_thickness = 2.f; /**< for the frame */

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
