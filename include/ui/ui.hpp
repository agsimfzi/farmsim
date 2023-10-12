#pragma once

#include <SFML/Graphics.hpp>

#include <deque>
#include <memory>

#include <game/game.hpp>

#include "ui_elements.hpp"
#include "ui_render.hpp"

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

    void interact();
    void useBuilding();
    void useNPC();

    void loadDefaultReactions();

    void readSeasonChange();

    void show();
    void hide();

    void showLayer(UI_Render layer);
    void hideLayer(UI_Render layer);

    bool layerHidden(UI_Render layer);

    void loadInventory();

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

    Dialogue dialogue;

    bool hidden = true;

    std::map<UI_Render, std::deque<sf::Drawable*>> renderer;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
