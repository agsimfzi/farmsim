#include <game/game_renderer.hpp>

const sf::Vector2i Game_Renderer::render_distance = sf::Vector2i(24, 16);

void Game_Renderer::clear()
{
    drawables.clear();
    items.clear();
}

void Game_Renderer::load(World& world, Player& player)
{
    clear();
    drawables.clear();
    items.clear();
    // 0 floor
    // 1 details
    // 2 world bits
    // 3 extra layer
    drawables.resize(4);

    Chunk_Loader& loader = world.getChunks();
    std::vector<Chunk*> chunks = loader.localChunks();
    std::vector<std::shared_ptr<Item>> items;
    for (const auto& c : chunks) {
        drawables[0].push_back(&c->getFloor());
        items.insert(items.end(), c->getItems().begin(), c->getItems().end());
        drawables[3].push_back(&c->getFrame());
    }
    Map_Tile<Crop>& crops = world.getCrops();
    std::vector<std::shared_ptr<Vehicle>>& vehicles = world.getVehicles();


    for (const auto& v : vehicles) {
        drawables[2].push_back(v.get());
    }

    for (auto& i : items) {
        drawables[2].push_back(i.get());
    }

    bool flying = (player.getState() == Entity_State::BROOMING);
    if (flying) {
        drawables[3].push_back(&player);
    }

    sf::Vector2i p(player.getCoordinates(tile_size));
    render_start = p - render_distance;
    render_end = p + render_distance;

    for (int y = render_start.y; y <= render_end.y; y++) {
        for (int x = render_start.x; x <= render_end.x; x++) {
            sf::Vector2i i(x, y);

            sf::Sprite* d = loader.detail(i);
            if (d) {
                int index = 1;
                if (world.getTileLibrary()[x][y].detail->getType() == Detail::TREE) {
                    index = 2;
                }
                drawables[index].push_back(d);
            }

            std::shared_ptr<Building> b = loader.building(i);
            if (b) {
                drawables[2].push_back(&b->sprite);
            }
            else if (crops.contains(x) && crops[x].contains(y)) {
                drawables[2].push_back(&crops[x][y]);
            }

        }

        if (!flying && y == p.y) {
            flying = true;
            drawables[2].push_back(&player);
        }
    }
}

void Game_Renderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& v : drawables) {
        for (const auto& d : v) {
            target.draw(*d, states);
        }
    }
}
