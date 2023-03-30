#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <game/game.hpp>

#include "ui_elements.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class UI : public sf::Drawable {
public:
    UI() = delete;

    UI(sf::RenderWindow& window, Game& game, sf::View& view);

    void scale(sf::RenderWindow& window);

    void update();

    void stopInput();

    void newLevel();

    void resize(sf::Vector2u windowSize);

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

    void useBuilding();

    void loadDefaultReactions();

    void readSeasonChange();

    void show();
    void hide();

private:
    sf::RenderWindow& window;
    Game& game;
    sf::Font& font;

    sf::View& view;

    sf::Text player_pos;

    sf::RectangleShape overlay;

    std::unique_ptr<Inventory_Interface> inventory_interface;

    Player_Target player_target;

    bool overlay_active = false;

    Minimap minimap;

    Game_Info game_info;

    bool hidden = true;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
