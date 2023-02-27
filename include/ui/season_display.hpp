#pragma once

class Season_Display : public sf::Drawable {
public:
    Season_Display();

private:
    sf::RectangleShape display;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
