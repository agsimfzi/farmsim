#include <ui/minimap.hpp>

#include <iostream>

#include <SFML/Window/Mouse.hpp>

Minimap::Minimap()
{
    map.setPrimitiveType(sf::Points);

    player_blip.setPointCount(4);
    player_blip.setFillColor(sf::Color(170,0,204));

    close();
}

void Minimap::load(World& world)
{
    std::map<Biome, sf::Color> colors;
        colors[Biome::GRASSLAND] = sf::Color(120, 215, 130);
        colors[Biome::FOREST] = sf::Color(30, 140, 40);
        colors[Biome::BEACH] = sf::Color(249, 222, 112);
        colors[Biome::OCEAN] = sf::Color(15, 15, 100);
        colors[Biome::LAKE] = sf::Color(125, 125, 250);
        colors[Biome::RIVER] = sf::Color::Magenta;//(50, 50, 125);

    Map_Tile<Floor_Info>& tiles = world.getTileLibrary();
    sf::Vector2i min = world.worldMin();
    sf::Vector2i max = world.worldMax();
    sf::Vector2i size;
        size.x = max.x * 2 + 1;
        size.y = max.y * 2 + 1;
    map.resize(size.x * size.y);
    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            if (tiles[x][y].floor == Floor_Type::NULL_TYPE) {
                continue;
            }
            sf::Vector2f pos(tiles[x][y].coordinates);
            map.append(sf::Vertex(pos, colors[tiles[x][y].biome]));
        }
    }
}

void Minimap::update(sf::Vector2i player_coordinates)
{
    if (expanded) {
        if (dragging) {
            checkDrag();
        }
    }
    else if (this->player_coordinates != player_coordinates) {
        //sf::Vector2i diff = player_coordinates - this->player_coordinates;
        this->player_coordinates = player_coordinates;
        //view.move(diff.x, diff.y);
        sf::Vector2f fpos(player_coordinates);
        view.setCenter(fpos);
        player_blip.setPosition(fpos);
    }
}

void Minimap::expand()
{
    sf::Vector2f size(1920.f, 1080.f);
    sf::Vector2f factor;
    factor.x = .8f;
    factor.y = .8f;
    view.setSize(size.x * factor.x, size.y * factor.y);
    sf::Vector2f pos;
    pos.x = (1.f - factor.x) / 2.f;
    pos.y = (1.f - factor.y) / 2.f;
    view.setViewport(sf::FloatRect(pos.x, pos.y, factor.x, factor.y));

    expanded = true;
    dragging = false;

    player_blip.setRadius(16.f);
}

void Minimap::close()
{
    sf::Vector2f size(1920.f, 1080.f);
    sf::Vector2f factor;
    factor.x = 0.15f;
    factor.y = (size.x * factor.x) / size.y;
    view.setSize(size.x * factor.x, size.y * factor.y);
    view.setCenter(player_coordinates.x, player_coordinates.y);
    view.setViewport(sf::FloatRect(1.f - factor.x, 0.f, factor.x, factor.y));

    expanded = false;
    dragging = false;

    player_blip.setRadius(4.f);
}

bool Minimap::isExpanded()
{
    return expanded;
}

void Minimap::startDrag()
{
    drag_pos = sf::Mouse::getPosition();
    dragging = true;
}

void Minimap::endDrag()
{
    dragging = false;
}

void Minimap::checkDrag()
{
    if (dragging) {
        sf::Vector2i mpos = sf::Mouse::getPosition();
        sf::Vector2i diff = drag_pos - mpos;
        view.move(diff.x, diff.y);
        drag_pos = mpos;
    }
}

void Minimap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.setView(view);
    target.draw(map, states);
    target.draw(player_blip);
}
