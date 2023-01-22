#pragma once

#include <memory>

#include <item/item.hpp>

class Inventory_Cell : public sf::Drawable {
public:
    Inventory_Cell(Item* item = nullptr);

    void setPosition(sf::Vector2f pos);

    const static sf::Vector2f frameSize;
    const static float frameOutlineSize;

    const static sf::Color colorActive;
    const static sf::Color colorInactive;
    const static sf::Color colorOutline;

    void activate();
    void deactivate();

    void setItem(Item* i);
    void clearItem();
    Item* getItem();

    template <typename T>
    bool contains(sf::Vector2<T> pos)
    {
        return frame.getGlobalBounds().contains(sf::Vector2f(pos));
    }

private:
    sf::RectangleShape frame;
    std::shared_ptr<Item> item = nullptr;

    sf::Text numberText;

    bool active = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
