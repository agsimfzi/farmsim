#include <world/chunk.hpp>

#include <resources/Texture_Manager.hpp>

#include <util/primordial.hpp>
#include <util/vector2_stream.hpp>

Chunk::Chunk(sf::Vector2i start, sf::Vector2i size, Map_Tile<Floor_Info>& info)
    : start { start }
{
    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            sf::Vector2i c(x, y);
            c += start;
            if (info[c.x][c.y].floor == Floor_Type::NULL_TYPE) {
                info[c.x][c.y].texture_pos = sf::Vector2i(0, (static_cast<int>(info[x][y].floor)) * roundFloat(Tile::tileSize));
                info[c.x][c.y].detail_pos = sf::Vector2i(0, 0);
            }
            std::string tkey = "FLOOR";
            Floor_Info& i = info[c.x][c.y];
            auto& f = floor[c.x][c.y];
            f = std::make_shared<Floor>(c, Texture_Manager::get(tkey));
            f->planted = i.planted;
            f->detail = i.detail;
            f->setType(i.floor);
            f->setTextureRect(sf::IntRect(i.texture_pos, sf::Vector2i(Tile::tileSize, Tile::tileSize)));

            if (i.detail != Detail_Type::NULL_TYPE) {
                tkey = detailTypeToString(i.detail);
                details[c.x][c.y] = std::make_shared<Detail>(c, i.detail, Texture_Manager::get(tkey), sf::IntRect(i.detail_pos, sf::Vector2i(Tile::tileSize, Tile::tileSize)));
            }
            if (i.tree) {
                tkey = "TREES";
                trees[c.x][c.y] = std::make_shared<Tree>(c, Texture_Manager::get(tkey));
            }
            else if (i.rock) {
                tkey = "ROCKS";
                rocks[c.x][c.y] = std::make_shared<Rock>(c, Texture_Manager::get(tkey));
            }
            else if (i.building) {
                addBuilding(i.building.get(), c);
            }
        }
    }
    sf::Vector2f f_offset(Tile::tileSize / 2.f, Tile::tileSize / 2.f);
    f_bounds = sf::FloatRect(sf::Vector2f(start) - f_offset, sf::Vector2f(size) * Tile::tileSize);
    i_bounds = sf::IntRect(start, size);

    sf::Vector2f frame_pos(floor[start.x][start.y]->getPosition());
    frame_pos -= f_offset;
    sf::Vector2f frame_size(size);
    frame_size *= Tile::tileSize;
    frame.setPosition(frame_pos);
    frame.setSize(frame_size);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(2.f);
    frame.setOutlineColor(sf::Color::Red);

    end = start + size;
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

Floor* Chunk::getFloor(sf::Vector2i i)
{
    Floor* t = nullptr;
    if (floor.contains(i.x) && floor[i.x].contains(i.y)) {
        t = floor[i.x][i.y].get();
    }
    return t;
}

Detail* Chunk::getDetail(sf::Vector2i i)
{
    Detail* t = nullptr;
    if (details.contains(i.x) && details[i.x].contains(i.y)) {
        t = details[i.x][i.y].get();
    }
    return t;
}

Tree* Chunk::getTree(sf::Vector2i i)
{
    Tree* t = nullptr;
    if (trees.contains(i.x) && trees[i.x].contains(i.y)) {
        t = trees[i.x][i.y].get();
    }
    return t;
}

Rock* Chunk::getRock(sf::Vector2i i)
{
    Rock* r = nullptr;
    if (rocks.contains(i.x) && rocks[i.x].contains(i.y)) {
        r = rocks[i.x][i.y].get();
    }
    return r;
}

sf::Sprite* Chunk::getBuilding(sf::Vector2i i)
{
    sf::Sprite* b = nullptr;
    if (buildings.contains(i.x) && buildings[i.x].contains(i.y)) {
        b = buildings[i.x][i.y].get();
    }
    return b;
}

void Chunk::eraseDetail(sf::Vector2i i)
{
    if (details.contains(i.x) && details[i.x].contains(i.y)) {
        details[i.x].erase(i.y);
    }
}

void Chunk::eraseTree(sf::Vector2i i)
{
    if (trees.contains(i.x) && trees[i.x].contains(i.y)) {
        trees[i.x].erase(i.y);
    }
}

void Chunk::eraseRock(sf::Vector2i i)
{
    if (rocks.contains(i.x) && rocks[i.x].contains(i.y)) {
        rocks[i.x].erase(i.y);
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

void Chunk::addBuilding(Building* building, sf::Vector2i c)
{
    sf::Sprite sprite;

    std::string texture = Building::typeToString(building->type);

    size_t sheet_id = building->uid % 1000;

    texture += std::to_string(sheet_id / 100);

    sheet_id %= 100;

    sprite.setTexture(Texture_Manager::get(texture));

    sf::Vector2i pos;
    pos.x = (sheet_id % 10) * 64;
    pos.y = (sheet_id / 10) * 64;
    sf::Vector2i size(64, 64);
    sprite.setTextureRect(sf::IntRect(pos, size));
    sprite.setOrigin(sf::Vector2f(size) / 2.f);
    sf::Vector2f p(c);
    p *= Tile::tileSize;
    sprite.setPosition(p);
    buildings[c.x][c.y] = std::make_shared<sf::Sprite>(sprite);
}

void Chunk::addItem(std::shared_ptr<Item> item, size_t count, sf::Vector2f pos)
{
    if (item && count > 0) {
        std::cout << "adding " << item->count() << " " << item->getName() << " to " << pos << '\n';
        items.push_back(item);
        items.back()->setCount(count);
        items.back()->setPosition(pos);
    }
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int x = i_bounds.left; x < i_bounds.left + i_bounds.width; x++) {
        for (int y = i_bounds.top; y < i_bounds.top + i_bounds.height; y++) {
            target.draw(*floor.at(x).at(y), states);
            if (details.contains(x) && details.at(x).contains(y)) {
                target.draw(*details.at(x).at(y), states);
            }
        }
    }

    for (int x = i_bounds.left; x < i_bounds.left + i_bounds.width; x++) {
        for (int y = i_bounds.top; y < i_bounds.top + i_bounds.height; y++) {
            if (trees.contains(x) && trees.at(x).contains(y) && trees.at(x).at(y)) {
                target.draw(*trees.at(x).at(y), states);
            }
            else if (rocks.contains(x) && rocks.at(x).contains(y) && rocks.at(x).at(y)) {
                target.draw(*rocks.at(x).at(y), states);
            }
            else if (buildings.contains(x) && buildings.at(x).contains(y) && buildings.at(x).at(y)) {
                target.draw(*buildings.at(x).at(y), states);
            }
        }
    }

    for (const auto& i : items) {
        target.draw(*i, states);
    }

    target.draw(frame, states);
}
