#include <ui/minimap.hpp>

#include <SFML/Window/Mouse.hpp>

#include <resources/palette.hpp>

Minimap::Minimap()
{
    player_blip.setPointCount(4);
    player_blip.setFillColor(sf::Color(170,0,204));

    mini_factor.x = mini_size.x / screen_size.x;
    mini_factor.y = mini_size.y / screen_size.y;

    full_factor.x = full_size.x / screen_size.x;
    full_factor.y = full_size.y / screen_size.y;

// close() MUST BE CALLED FOR INITIALIZATION
// A SEGFAULT IS GUARANTEED OTHERWISE!
    close();

    sf::Vector2f frame_offset(frame_border_size, frame_border_size);

    mini_frame.setFillColor(Palette::inventory_bg);
    mini_frame.setOutlineColor(Palette::inventory_outline);
    mini_frame.setOutlineThickness(frame_border_size);

    mini_frame.setSize(mini_size - (2.f * frame_offset));
    mini_frame.setPosition(screen_size.x - mini_size.x + frame_offset.x, frame_offset.y);

    full_frame.setFillColor(Palette::inventory_bg);
    full_frame.setOutlineColor(Palette::inventory_outline);
    full_frame.setOutlineThickness(frame_border_size);

    full_frame.setSize(full_size - (2.f * frame_offset));
    full_frame.setPosition(((screen_size - full_size) / 2.f) + frame_offset);
}

void Minimap::load(World& world)
{
    sf::VertexArray map_array;
    map_array.setPrimitiveType(sf::Points);

    std::map<Biome, sf::Color> colors;
        colors[Biome::GRASSLAND] = sf::Color(120, 215, 130);
        colors[Biome::FOREST] = sf::Color(30, 140, 40);
        colors[Biome::BEACH] = sf::Color(249, 222, 112);
        colors[Biome::OCEAN] = sf::Color(15, 15, 100);
        colors[Biome::LAKE] = sf::Color(125, 125, 250);
        colors[Biome::RIVER] = sf::Color::Magenta;//(50, 50, 125);
        colors[Biome::VOLCANO] = sf::Color(88, 77, 77);
        colors[Biome::CALDERA] = sf::Color(220, 108, 16);
        colors[Biome::NULL_TYPE] = sf::Color::White;

    Map_Tile<Floor_Info>& tiles = world.getTileLibrary();
    sf::Vector2i min = world.worldMin();
    sf::Vector2i max = world.worldMax();
    sf::Vector2i size;
        size.x = max.x * 2 + 1;
        size.y = max.y * 2 + 1;
    size = max - min;
    map_array.resize(size.x * size.y);
    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            sf::Vector2f pos(tiles[x][y].coordinates + (size / 2));
                map_array.append(sf::Vertex(pos, colors[tiles[x][y].biome]));
        }
    }

// convert VertexArray to a texture
// from https://en.sfml-dev.org/forums/index.php?topic=13799.0
    sf::RenderTexture rt;
    rt.create(size.x, size.y);
    rt.draw(map_array);
    rt.display();

    sf::Image mimg;
    mimg = rt.getTexture().copyToImage();
    mimg.createMaskFromColor(colors[Biome::NULL_TYPE]);

    texture.loadFromImage(mimg);
    map.setTexture(texture);

    sf::Vector2f center;
    center.x = map.getGlobalBounds().width / 2.f;
    center.y = map.getGlobalBounds().height / 2.f;
    map.setOrigin(center);
    map.setPosition(sf::Vector2f(0.f, 0.f));
}

void Minimap::update(sf::Vector2i player_coordinates)
{
    if (expanded && dragging) {
        checkDrag();
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
    view.setSize(full_size);
    view.setCenter(player_coordinates.x, player_coordinates.y);
    view.setViewport(sf::FloatRect((1.f - full_factor.x) / 2.f
                                 , (1.f - full_factor.y) / 2.f
                                 , full_factor.x
                                 , full_factor.y));

    expanded = true;

    float r = 12.f;
    player_blip.setRadius(r);
    player_blip.setOrigin(r, r);

    float factor = 0.33f;
    view.zoom(factor);
    zoom_level = factor;

    active_frame = &full_frame;
}

void Minimap::close()
{
    view.setSize(mini_size);
    view.setCenter(player_coordinates.x, player_coordinates.y);
    view.setViewport(sf::FloatRect(1.f - mini_factor.x, 0.f, mini_factor.x, mini_factor.y));

    float factor = 0.67f;
    view.zoom(factor);
    zoom_level = factor;

    expanded = false;
    dragging = false;

    float r = 4.f;
    player_blip.setRadius(r);
    player_blip.setOrigin(r, r);

    active_frame = &mini_frame;
}

bool Minimap::isExpanded()
{
    return expanded;
}

void Minimap::startDrag()
{
    if (expanded && !dragging) {
        drag_pos = sf::Mouse::getPosition();
        dragging = true;
    }
}

void Minimap::endDrag()
{
    dragging = false;
}

void Minimap::checkDrag()
{
    if (dragging) {
        sf::Vector2i mpos = sf::Mouse::getPosition();
        sf::Vector2f diff(drag_pos - mpos);
        diff *= zoom_level;
        view.move(diff);
        drag_pos = mpos;
    }
}

void Minimap::zoom(float delta)
{
    if (delta < 0.f) {
        float factor = 1.1f;
        view.zoom(factor);
        zoom_level *= factor;
    }
    else if (delta > 0.f) {
        float factor = 0.9f;
        view.zoom(factor);
        zoom_level *= factor;
    }
}

void Minimap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*active_frame, states);
    target.setView(view);
    target.draw(map, states);
    target.draw(player_blip, states);
}
