#include <world/chunk.hpp>

#include <resources/Texture_Manager.hpp>

#include <util/primordial.hpp>

Chunk::Chunk(sf::Vector2i start, sf::Vector2i size, Map_Tile<Tile_Info>& info)
    : start { start }
    , end { start + size }
{
    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            sf::Vector2i c(x, y);
            c += start;
            readTile(info[c.x][c.y]);
        }
    }
    sf::Vector2f f_offset(tile_size / 2.f, tile_size / 2.f);
    f_bounds = sf::FloatRect(sf::Vector2f(start) - f_offset, sf::Vector2f(size) * tile_size);
    i_bounds = sf::IntRect(start, size);

    sf::Vector2f frame_pos(floor[start.x][start.y].getPosition());
    frame_pos -= f_offset;
    sf::Vector2f frame_size(size);
    frame_size *= tile_size;
    frame.setPosition(frame_pos);
    frame.setSize(frame_size);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(2.f);
    frame.setOutlineColor(sf::Color::Red);
}

void Chunk::readTile(Tile_Info& info)
{
    sf::Vector2i c = info.coordinates;
    sf::Vector2f pos(info.coordinates);
    pos *= tile_size;
    if (info.floor == Floor_Type::NULL_TYPE) {
        info.texture_pos = sf::Vector2i(0, (static_cast<int>(info.floor)) * roundFloat(tile_size));
    }
    std::string tkey = "FLOOR";
    auto& f = floor[c.x][c.y];
    f.setTexture(Texture_Manager::get(tkey));
    f.setTextureRect(sf::IntRect(info.texture_pos, sf::Vector2i(tile_size, tile_size)));
    f.setOrigin(sf::Vector2f(tile_size / 2.f, tile_size / 2.f));
    f.setPosition(pos);

    if (info.building) {
        addBuilding(info.building, c);
    }
    else if (info.detail) {
        tkey = Detail::typeToString(info.detail->getType());
        details[c.x][c.y].setTexture(Texture_Manager::get(tkey));
        details[c.x][c.y].setTextureRect(info.detail->getTextureRect());
        sf::Vector2f origin;
        origin.x = info.detail->getTextureRect().width / 2;
        origin.y = info.detail->getTextureRect().height - (tile_size / 2.f);
        details[c.x][c.y].setOrigin(origin);
        details[c.x][c.y].setPosition(pos);
    }
}

void Chunk::update()
{
    for (auto& r : buildings) {
        for (auto& c : r.second) {
            if (c.second) {
                c.second->updateSprite();
            }
        }
    }
}

bool Chunk::contains(sf::Vector2f pos)
{
    return f_bounds.contains(pos);
}

bool Chunk::contains(sf::Vector2i coords)
{
    return (floor.contains(coords.x) && floor[coords.x].contains(coords.y));
    return i_bounds.contains(coords);
}

sf::Sprite* Chunk::getFloor(sf::Vector2i i)
{
    sf::Sprite* f = nullptr;
    if (floor.contains(i.x) && floor[i.x].contains(i.y)) {
        f = &floor[i.x][i.y];
    }
    return f;
}

sf::Sprite* Chunk::detail(sf::Vector2i i)
{
    sf::Sprite* d = nullptr;
    if (details.contains(i.x) && details[i.x].contains(i.y)) {
        d = &details[i.x][i.y];
    }
    return d;
}

std::shared_ptr<Building> Chunk::getBuilding(sf::Vector2i i)
{
    return buildings[i.x][i.y];
}

void Chunk::eraseDetail(sf::Vector2i i)
{
    if (details.contains(i.x) && details[i.x].contains(i.y)) {
        details[i.x].erase(i.y);
    }
}

void Chunk::eraseBuilding(sf::Vector2i i)
{
    if (buildings.contains(i.x) && buildings[i.x].contains(i.y)) {
        buildings[i.x].erase(i.y);
    }
}

std::vector<std::shared_ptr<Item>>& Chunk::getItems()
{
    return items;
}

void Chunk::addBuilding(std::shared_ptr<Building> b, sf::Vector2i c)
{
    b->sprite.setPosition(floor[c.x][c.y].getPosition());
    buildings[c.x][c.y] = b;
}

void Chunk::addItem(std::shared_ptr<Item> item, sf::Vector2f pos)
{
    if (item) {
        items.push_back(item);
        items.back()->setPosition(pos);
    }
}

Map_Tile<sf::Sprite>& Chunk::getFloorMap()
{
    return floor;
}

Map_Tile<sf::Sprite>& Chunk::getDetails()
{
    return details;
}

Map_Tile<std::shared_ptr<Building>>& Chunk::getBuildings()
{
    return buildings;
}

sf::RectangleShape& Chunk::getFrame()
{
    return frame;
}
