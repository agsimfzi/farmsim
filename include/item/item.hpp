#pragma once

#include <SFML/Graphics.hpp>

#include "item_data.hpp"
#include "item_type.hpp"

class Item : public sf::Drawable, private Item_Data {
public:
    Item() = default;
    Item(Item_Data d);

    void setSprite(sf::Sprite sprite);

    void setPosition(sf::Vector2f pos);

    bool sellable();
    bool stackable();
    int stackSize();

    std::string getName();
    size_t getUID();
    Item_Type getType();

    sf::Sprite& getSprite();

    size_t add(size_t diff);
    size_t take(size_t diff);

    size_t count();
    void setCount(size_t n_count);

    bool useLimit();
    void resetUses();
    int usePercent();
    void reduceUses();
    int useFactor();

    bool can_pickup = true;

    sf::Vector2f getPosition();

    std::string getDescription();
    int getValue();

    bool edible();

private:
    sf::Sprite sprite;

    size_t m_count;
    bool m_edible = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
