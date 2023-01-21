#pragma once

#include <SFML/Graphics.hpp>

#include <entity/entity.hpp>

class Entity_Mouseover : public sf::Drawable {
public:
    Entity_Mouseover(const sf::Font& font);

    void update(Entity* entity);

    void set(sf::Vector2f framePos, sf::Vector2f size);

private:
    bool opened { false };

    sf::Sprite sprite;

    sf::Text nameText;
    sf::Text descriptionText;

    sf::RectangleShape frame;

    sf::Vector2f spritePos;

    bool isOpen();
    void open(Entity* entity);
    void close();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
