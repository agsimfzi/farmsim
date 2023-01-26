#include <ui/ui.hpp>

#include <resources/texture_manager.hpp>
#include <resources/font_manager.hpp>

#include <util/fmouse.hpp>

//////////////////////////////////////////////////////////////

UI::UI(sf::RenderWindow& window, Game& game)
    : window { window }
    , game { game }
    , font { Font_Manager::get(Font::UI) }
    , entityInfo { Font_Manager::get(Font::UI) }
    , inventory_interface { Inventory_Interface(game.getInventory()) }
{
    overlay.setPosition(sf::Vector2f(0.f, 0.f));
    overlay.setSize(sf::Vector2f(1920.f, 1080.f));
    overlay.setFillColor(sf::Color(50, 50, 50, 120));

    player_pos.setPosition(sf::Vector2f(1800.f, 1000.f));
    player_pos.setFont(font);
    player_pos.setFillColor(sf::Color(200, 200, 200));
}

void UI::init()
{
    inventory_interface.readInventory();
}

void UI::update()
{
    if (inventory_interface.expanded) {
        inventory_interface.checkDrag();
    }
    inventory_interface.pollChanges();

    std::string p = std::to_string(game.getPlayer().getCoordinates(Tile::tileSize).x)
                  + ','
                  + std::to_string(game.getPlayer().getCoordinates(Tile::tileSize).y);

    player_pos.setString(p);

    sf::Vector2i* coords = game.getWorld().checkMouseTarget(fMouse(window, game.getView()), game.getPlayer().getCoordinates(Tile::tileSize));
    if (coords) {
        sf::Vector2i c = window.mapCoordsToPixel(sf::Vector2f(*coords) * Tile::tileSize, game.getView());
        player_target.set(c);
    }
    else {
        player_target.active = false;
    }
    //

}

void UI::setMouseover(Entity* entity)
{
    entityInfo.update(entity);
}

void UI::scroll(float delta)
{
    if (!overlay_active && !inventory_interface.expanded) {
        // parse "reverse inventory_interface scroll" setting
        if (delta > 0.f) {
            inventory_interface.scrollLeft();
        }
        else if (delta < 0.f) {
            inventory_interface.scrollRight();
        }

        game.getInventory().setEquipped(inventory_interface.getEquippedIndex());
    }
}

void UI::numRelease(int num)
{
    if (!overlay_active && !inventory_interface.expanded) {
        inventory_interface.setEquippedIndex(num);

        game.getInventory().setEquipped(inventory_interface.getEquippedIndex());
    }
}

void UI::scale(sf::RenderWindow& window)
{
    sf::Vector2f pos(0.f, 0.f);
    pos.x *= window.getSize().x;
    pos.y *= window.getSize().y;
    sf::Vector2f size(256, 256);
    pos.y += size.y + 8.f;
    //size.y = 320.f;

    entityInfo.set(pos, size);
}

void UI::stopInput()
{
    game.stopInput();
}

void UI::resize(sf::Vector2u windowSize)
{
}

void UI::toggleInventory()
{
    if (inventory_interface.expanded) {
        inventory_interface.expanded = false;
        overlay_active = false;
    }
    else {
        inventory_interface.expanded = true;
        overlay_active = true;
    }
}

void UI::closeOverlay()
{
    overlay_active = false;
    if (inventory_interface.expanded) {
        inventory_interface.close();
    }
}

bool UI::overlayActive()
{
    return overlay_active;
}

bool UI::clickLeft()
{
    bool parsed = overlay_active;

    if (parsed) {
        inventory_interface.startDrag();
    }

    return parsed;
}

bool UI::releaseLeft()
{
    bool parsed = overlay_active;

    if (parsed) {
        inventory_interface.endDrag();
    }

    return parsed;
}

bool UI::clickRight()
{
    return overlay_active;
}

bool UI::releaseRight()
{
    bool parsed = overlay_active;

    if (parsed) {
        closeOverlay();
    }

    return parsed;
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(entityInfo, states);

    if (overlay_active) {
        target.draw(overlay, states);
    }
    else {
        target.draw(player_target, states);
    }

    target.draw(inventory_interface, states);

    target.draw(player_pos, states);
}
