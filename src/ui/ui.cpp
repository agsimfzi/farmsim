#include <ui/ui.hpp>

#include <resources/font_manager.hpp>
#include <resources/palette.hpp>
#include <resources/texture_manager.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>
#include <util/shift_pressed.hpp>

//////////////////////////////////////////////////////////////

UI::UI(sf::RenderWindow& window, Game& game, sf::View& view)
    : window { window }
    , game { game }
    , font { Font_Manager::get(Font::UI) }
    , view{ view }
{
    inventory_interface = std::make_unique<Inventory_Interface>(Inventory_Interface(game.getInventory(), view));
    auto drop = [&](std::shared_ptr<Item> i)
        {
            i->can_pickup = false;
            game.getWorld().getChunks().addItem(i, game.getPlayer().getCoordinates(tile_size));
        };

    inventory_interface->loadDrop(drop);

    overlay.setPosition(sf::Vector2f(0.f, 0.f));
    overlay.setSize(sf::Vector2f(1920.f, 1080.f));
    overlay.setFillColor(sf::Color(50, 50, 50, 120));

    player_pos.setPosition(sf::Vector2f(1800.f, 1000.f));
    player_pos.setFont(font);
    player_pos.setFillColor(Palette::white);
    player_pos.setCharacterSize(12);
    player_pos.setOutlineThickness(2.f);
    player_pos.setOutlineColor(Palette::black);

    game_info.season.setTexture(&Texture_Manager::get("SEASONS"));
    game_info.season.setSize(sf::Vector2f(64.f, 32.f));
    game_info.season.setOutlineThickness(1.f);
    game_info.season.setOutlineColor(Palette::black);
    sf::Vector2f pos;
    pos.x = window.getSize().x - game_info.season.getSize().x - 8.f;
    pos.y = minimap.getSize().y + 8.f;
    game_info.season.setPosition(pos);
    readSeasonChange();

    pos.x += game_info.season.getSize().x;
    pos.y += game_info.season.getSize().y + (game_info.wallet.getSize().y / 2.f) + 4.f;

    game_info.wallet.setPosition(pos);

    pos = sf::Vector2f(window.getSize()) - sf::Vector2f(8.f, 8.f);
    pos -= game_info.energy.getSize();

    game_info.energy.setPosition(sf::Vector2f(1600.f, 980.f));

    auto update_wallet = std::bind(&Wallet_Inspector::update, &game_info.wallet, std::placeholders::_1);
    game.getPlayer().getWallet().update_ui = update_wallet;
}

void UI::init()
{
    minimap.load(game.getWorld());
}

void UI::update()
{
    if (inventory_interface) {
        inventory_interface->update(window);
    }

    std::string p = std::to_string(game.getPlayer().getCoordinates(tile_size).x)
                  + ','
                  + std::to_string(game.getPlayer().getCoordinates(tile_size).y);

    player_pos.setString(p);
    sf::Vector2i* coords = game.getWorld().checkMouseTarget(fMouse(window, game.getView()), game.getPlayer().getCoordinates(tile_size));
    if (coords) {
        sf::Vector2i c = window.mapCoordsToPixel(sf::Vector2f(*coords) * tile_size, game.getView());
        player_target.set(c);
    }
    else {
        player_target.active = false;
    }

    minimap.update(game.getPlayer().getCoordinates(tile_size));

    int energy = game.playerEnergy();
    if (game_info.last_player_energy != energy) {
        game_info.last_player_energy = energy;
        game_info.energy.update(game.energyPercent());
    }

}

void UI::scroll(float delta)
{
    if (overlay_active && inventory_interface->open) {
    }
    if (overlay_active && minimap.isExpanded()) {
        minimap.zoom(delta);
    }
    else {
        // parse "reverse inventory_interface scroll" setting (invert delta's sign)
        if (inventory_interface->scroll(delta, window)) {
            game.getInventory().setEquipped(inventory_interface->getEquippedIndex());
            game.stopUse();
        }
    }
}

void UI::numRelease(int num)
{
    if (!overlay_active && !inventory_interface->open) {
        inventory_interface->setEquippedIndex(num);

        game.getInventory().setEquipped(inventory_interface->getEquippedIndex());
        game.stopInput();
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
}

void UI::stopInput()
{
    game.stopInput();
}

void UI::resize(sf::Vector2u windowSize)
{
}

void UI::loadDefaultReactions()
{
    inventory_interface->loadReactions(game.getLibrary().building("null_crafting")->reactions, game.getLibrary());
}

void UI::toggleInventory()
{
    if (inventory_interface->open) {
        inventory_interface->close();
        overlay_active = false;
        game.getWorld().closeActiveBuilding();
        inventory_interface = std::make_unique<Inventory_Interface>(Inventory_Interface(game.getInventory(), view));
    }
    else if (!overlay_active) {
        inventory_interface->open = true;
        inventory_interface = std::make_unique<Inventory_Interface>(Inventory_Interface(game.getInventory(), view));
        loadDefaultReactions();
        inventory_interface->open = true;
        overlay_active = true;
    }
}

void UI::toggleMap()
{
    if (minimap.isExpanded()) {
        minimap.close();
        overlay_active = false;
    }
    else if (!overlay_active) {
        minimap.expand();
        overlay_active = true;
    }
}

void UI::closeOverlay()
{
    overlay_active = false;
    if (inventory_interface->open) {
        inventory_interface->close();
        inventory_interface = std::make_unique<Inventory_Interface>(*inventory_interface);
    }
    else if(minimap.isExpanded()) {
        minimap.close();
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
        if (inventory_interface->open) {
            if (!inventory_interface->dragging && shiftPressed()) {
                inventory_interface->shiftClickLeft();
            }
            else {
                inventory_interface->clickLeft(window);
            }
        }
        else if(minimap.isExpanded()) {
            minimap.startDrag();
        }
    }

    return parsed;
}

bool UI::releaseLeft()
{
    bool parsed = overlay_active;

    if (parsed) {
        if(minimap.isExpanded()) {
            minimap.endDrag();
        }
    }

    return parsed;
}

bool UI::clickRight()
{
    bool parsed = overlay_active;
    if (parsed) {
        if (inventory_interface->open) {
            if (!inventory_interface->dragging && shiftPressed()) {
                inventory_interface->shiftClickRight();
            }
            else {
                inventory_interface->clickRight();
            }
        }
        else if (minimap.isExpanded()) {
            closeOverlay();
        }
    }
    return parsed;
}

bool UI::releaseRight()
{
    bool parsed = overlay_active;

    return parsed;
}

void UI::useBuilding()
{
    if (!overlay_active) {
        game.getWorld().setActiveBuilding();
        Building* b = game.getWorld().activeBuilding();
        if (b) {
            switch(b->type) {
                default:
                    return;
                case Building::CONTAINER:
                    inventory_interface = std::make_unique<Container_Interface>(game.getInventory(), view, dynamic_cast<Container*>(b));
                    inventory_interface->open = true;
                    loadDefaultReactions();
                    break;
                case Building::CRAFTING:
                    inventory_interface = std::make_unique<Inventory_Interface>(Inventory_Interface(game.getInventory(), view));
                    inventory_interface->loadReactions(b->reactions, game.getLibrary());
                    inventory_interface->open = true;
                    break;
                case Building::MACHINE:
                    inventory_interface = std::make_unique<Machine_Interface>(game.getInventory(), view, dynamic_cast<Machine*>(b));
                    inventory_interface->loadReactions(b->reactions, game.getLibrary());
                    inventory_interface->open = true;
                    break;
                case Building::FURNITURE:
                    if (equalStrings(b->name, "bed")) {
                        game.setState(Game_State::FADE_OUT);
                    }
                    return;
            }
            inventory_interface->open = true;
            overlay_active = true;
        }
    }
    else {
        closeOverlay();
        inventory_interface->close();
        overlay_active = false;
        game.getWorld().closeActiveBuilding();
    }
}

void UI::readSeasonChange()
{
    sf::IntRect trect(0, 0, 64, 32);
    trect.left = static_cast<int>(game.getWorld().getSeason()) * 64;
    game_info.season.setTextureRect(trect);
}

void UI::hide()
{
    hidden = true;
}

void UI::show()
{
    hidden = false;
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!hidden) {
        if (overlay_active) {
            target.draw(overlay, states);
        }
        else {
            target.draw(player_target, states);
        }

        target.draw(player_pos, states);

        target.draw(game_info, states);

        target.draw(*inventory_interface, states); // reaction_interface view

        target.draw(minimap, states); // MUST BE LAST AS IT DEFINES ITS OWN VIEW!
    }
}
