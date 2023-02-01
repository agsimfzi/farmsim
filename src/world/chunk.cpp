#include <world/chunk.hpp>

#include <resources/Texture_Manager.hpp>

Chunk::Chunk(sf::Vector2i start, sf::Vector2i size, Map_Tile<Floor_Info>& info)
    : start { start }
{
    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            sf::Vector2i c(x, y);
            c += start;
            std::string tkey = "FLOOR";
            if (info[c.x][c.y].floor == Floor_Type::WATER) {
                tkey = "WATER";
            }
            floor[c.x][c.y] = std::make_shared<Floor>(c, Texture_Manager::get(tkey));
            floor[c.x][c.y]->planted = info[c.x][c.y].planted;
            floor[c.x][c.y]->detail = info[c.x][c.y].detail;
            floor[c.x][c.y]->setType(info[c.x][c.y].floor);
            floor[c.x][c.y]->setTextureRect(sf::IntRect(info[c.x][c.y].texture_pos, sf::Vector2i(Tile::tileSize, Tile::tileSize)));

            if (info[c.x][c.y].detail == Detail_Type::GRASS) {
                tkey = "GRASS";
                details[c.x][c.y] = std::make_shared<Detail>(c, info[c.x][c.y].detail, Texture_Manager::get(tkey));
                details[c.x][c.y]->setTextureRect(sf::IntRect(info[c.x][c.y].detail_pos, sf::Vector2i(Tile::tileSize, Tile::tileSize)));
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

void Chunk::eraseDetail(sf::Vector2i i)
{
    if (details.contains(i.x) && details[i.x].contains(i.y)) {
        details[i.x].erase(i.y);
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
    target.draw(frame, states);
}
