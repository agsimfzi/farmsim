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
    Map_Tile<Floor>& floor = world.getFloor();
    sf::Vector2i world_min = world.worldMin();
    sf::Vector2i world_max = world.worldMax();
    sf::Vector2i size;
        size.x = world_max.x * 2 + 1;
        size.y = world_max.y * 2 + 1;
    map.resize(size.x * size.y);
    for (int x = world_min.x; x <= world_max.x; x++) {
        for (int y = world_min.y; y <= world_max.y; y++) {
            Floor* f = floor[x][y].get();
            if (f) {
                sf::Vector2f pos(f->coordinates);
                sf::Color color;
                switch (f->type) {
                case Floor_Type::WATERED:
                case Floor_Type::TILLED:
                case Floor_Type::DIRT:
                    if (f->details.size() > 0
                    && f->details.front().type == Detail_Type::GRASS) {
                        color = sf::Color(20, 250, 40, 255);
                    }
                    else {
                        color = sf::Color(166, 135, 93, 255);
                    }
                    break;
                case Floor_Type::WATER:
                    color = sf::Color(40, 20, 250, 255);
                    break;
                default:
                    color = sf::Color(255, 255, 255, 255);
                    break;
                }
                map.append(sf::Vertex(pos, color));
            }
            else {
                std::cout << "\tno floor found at " << x << ", " << y << "!\n";
            }
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
