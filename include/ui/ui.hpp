#pragma once

#include <SFML/Graphics.hpp>

#include <game/game.hpp>

#include "ui_elements.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class UI : public sf::Drawable {
public:
    UI(Game& game);

    void scale(sf::RenderWindow& window);

    void update();

    void stopInput();

    void newLevel();

    void resize(sf::Vector2u windowSize);

    void setMouseover(Entity* entity);

    void scroll(float delta);

    void numRelease(int num);

    void init();

    void openInventory();
    void closeOverlay();

    bool overlayActive();
    void setOverlay(bool active);

private:
    Game& game;
    sf::Font& font;

    Entity_Mouseover entityInfo;

    sf::RectangleShape overlay;

    Quickbar quickbar;

    bool overlay_active = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
