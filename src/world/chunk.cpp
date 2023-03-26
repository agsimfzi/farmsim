#include <world/chunk.hpp>

#include <resources/Texture_Manager.hpp>

#include <util/primordial.hpp>

Chunk::Chunk(sf::Vector2i start, sf::Vector2i size, Map_Tile<Tile>& info)
    : start { start }
    , end { start + size }
    , size { size }
{
    floor.load(info, start, size);
    f_bounds = sf::FloatRect(sf::Vector2f(start) * tile_size, sf::Vector2f(size) * tile_size);
    i_bounds = sf::IntRect(start, size);

    sf::Vector2f frame_pos;//(floor[start.x][start.y].getPosition());
    sf::Vertex* quad = &floor.vertices[0];

    frame_pos = quad[0].position + (sf::Vector2f(start) * tile_size);
    sf::Vector2f frame_size(size);
    frame_size *= tile_size;
    frame.setPosition(frame_pos);
    frame.setSize(frame_size);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(2.f);
    frame.setOutlineColor(sf::Color::Red);

    for (int x = start.x; x < end.x; x++) {
        for (int y = start.y; y < end.y; y++) {
            readTile(info[x][y]);
        }
    }
}

void Chunk::readTile(Tile& info)
{
    sf::Vector2i c = info.coordinates;
    sf::Vector2f pos(info.coordinates);
    pos *= tile_size;
    if (info.floor == Floor_Type::NULL_TYPE) {
        info.texture_pos = sf::Vector2i(0, (static_cast<int>(info.floor)) * roundFloat(tile_size));
    }
    std::string tkey = "FLOOR";
    sf::Vector2i i = info.coordinates - start;

    floor.change(i.x + i.y * size.x, info.texture_pos, size);

    if (info.building) {
        addBuilding(info.building, c);
    }
    else if (info.detail) {
        tkey = Detail::typeToString(info.detail->getType());
        details[c.x][c.y].setTexture(Texture_Manager::get(tkey));
        details[c.x][c.y].setTextureRect(info.detail->getTextureRect());
        sf::Vector2f origin;
        origin.x = (info.detail->getTextureRect().width - tile_size) / 2.f;
        origin.y = info.detail->getTextureRect().height - tile_size;
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
    return i_bounds.contains(coords);
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
    b->sprite.setPosition(sf::Vector2f(c) * tile_size);
    buildings[c.x][c.y] = b;
}

void Chunk::addItem(std::shared_ptr<Item> item, sf::Vector2f pos)
{
    if (item) {
        items.push_back(item);
        items.back()->setPosition(pos);
    }
}

Floor_Map& Chunk::getFloor()
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
