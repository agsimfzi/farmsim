#include <ui/ui.hpp>

#include <resources/texture_manager.hpp>

#include <resources/font_manager.hpp>

//////////////////////////////////////////////////////////////

UI::UI(Game& game)
    : game { game }
    , font{ Font_Manager::get(Font::UI) }
    , entityInfo{ Font_Manager::get(Font::UI) }
    , quickbar{ Quickbar(game.getInventory()) }
{}

void UI::init()
{
    quickbar.readInventory();
}

void UI::update()
{
    quickbar.pollChanges();
}

void UI::setMouseover(Entity* entity)
{
    entityInfo.update(entity);
}

void UI::scroll(float delta)
{
    // parse "reverse quickbar scroll" setting
    if (delta > 0.f) {
        quickbar.scrollLeft();
    }
    else if (delta < 0.f) {
        quickbar.scrollRight();
    }

    game.getInventory().setEquipped(quickbar.getActiveIndex());
}

void UI::numRelease(int num)
{
    quickbar.setActiveCell(num);

    game.getInventory().setEquipped(quickbar.getActiveIndex());
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
{}

void UI::openInventory()
{
    overlay_active = true;
}

void UI::closeOverlay()
{
    overlay_active = false;
}

bool UI::overlayActive()
{
    return overlay_active;
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(entityInfo, states);

    if (overlay_active) {
        target.draw(overlay, states);
    }

    target.draw(quickbar, states);

}
