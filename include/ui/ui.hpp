#pragma once

#include <SFML/Graphics.hpp>

#include <game/game.hpp>

#include "ui_elements.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class UI : public sf::Drawable {
public:
    UI(sf::RenderWindow& window, Game& game);

    void scale(sf::RenderWindow& window);

    void update();

    void stopInput();

    void newLevel();

    void resize(sf::Vector2u windowSize);

    void setMouseover(Entity* entity);

    void scroll(float delta);

    void numRelease(int num);

    void init();

    void toggleInventory();
    void toggleMap();
    void closeOverlay();

    bool overlayActive();
    void setOverlay(bool active);

    bool clickLeft();
    bool releaseLeft();

    bool clickRight();
    bool releaseRight();

private:
    sf::RenderWindow& window;
    Game& game;
    sf::Font& font;

    Entity_Mouseover entityInfo;

    sf::Text player_pos;

    sf::RectangleShape overlay;

    Inventory_Interface inventory_interface;

    Player_Target player_target;

    bool overlay_active = false;

    Minimap minimap;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
