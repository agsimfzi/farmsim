#pragma once

#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////
/// \brief Button class for mouse navigation.
///
class Button : public sf::Drawable {
public:
    Button(std::string nlabel, sf::Font& font);

    void update(sf::Vector2f& mpos);
    bool isHighlighted();
    void highlight();
    void unhighlight();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition();

protected:
    sf::RectangleShape container;
    sf::Text label;

    bool highlighted = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    const static float padding;
};
