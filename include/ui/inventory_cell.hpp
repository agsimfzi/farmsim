#pragma once

#include <memory>

#include <item/item.hpp>

class Inventory_Cell : public sf::Drawable {
public:
    Inventory_Cell(std::shared_ptr<Item> item = nullptr);

    void setPosition(sf::Vector2f pos);

    const static sf::Vector2f frameSize;
    const static float frameOutlineSize;

    const static sf::Color colorActive;
    const static sf::Color colorInactive;
    const static sf::Color colorOutline;
    const static sf::Color colorUseBar;

    void activate();
    void deactivate();

    void setItem(std::shared_ptr<Item> i);
    void clearItem();
    std::shared_ptr<Item> getItem();

    void updateCount();
    void setCount(size_t count);

    template <typename T>
    bool contains(sf::Vector2<T> pos)
    {
        return frame.getGlobalBounds().contains(sf::Vector2f(pos));
    }

    sf::Vector2f getPosition();

private:
    sf::RectangleShape frame;
    std::shared_ptr<Item> item = nullptr;

    sf::Text numberText;

    bool active = false;

    sf::RectangleShape use_bar;
    const static sf::Vector2f use_bar_size;

    bool usable = false;

    void calculateUseBarSize(int percent);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
