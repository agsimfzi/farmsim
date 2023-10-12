#include <game/game_renderer.hpp>

const sf::Vector2i Game_Renderer::render_distance = sf::Vector2i(24, 16);

void Game_Renderer::clear()
{
    drawables.clear();
    items.clear();
}

void Game_Renderer::load(World& world, Player& player)
{
    for (int i = FLOOR; i < FADE; i++) {
        drawables[static_cast<Layer>(i)].clear();
    }
    items.clear();

    Chunk_Loader& loader = world.getChunks();
    std::vector<Chunk*> chunks = loader.localChunks();
    std::vector<std::shared_ptr<Item>> items;
    for (const auto& c : chunks) {
        drawables[FLOOR].push_back(&c->getFloor());
        items.insert(items.end(), c->getItems().begin(), c->getItems().end());
        drawables[EXTRA].push_back(&c->getFrame());
    }
    Map_Tile<Crop>& crops = world.getCrops();
    std::vector<std::shared_ptr<Vehicle>>& vehicles = world.getVehicles();


    for (const auto& v : vehicles) {
        drawables[BITS].push_back(v.get());
    }

    for (auto& i : items) {
        drawables[BITS].push_back(i.get());
    }

    bool flying = (player.getState() == Entity_State::BROOMING);
    if (flying) {
        drawables[EXTRA].push_back(&player);
    }

    sf::Vector2i p(player.getCoordinates(tile_size));
    render_start = p - render_distance;
    render_end = p + render_distance;

    auto& tile_library = world.getTileLibrary();

    for (int y = render_start.y; y <= render_end.y; y++) {
        for (int x = render_start.x; x <= render_end.x; x++) {
            sf::Vector2i i(x, y);

            sf::Sprite* d = loader.detail(i);
            if (d) {
                Layer layer = DETAILS;
                if (tile_library[x][y].detail->getType() == Detail::TREE) {
                    layer = BITS;
                }
                drawables[layer].push_back(d);
            }

            std::shared_ptr<Building> b = loader.building(i);
            if (b) {
                drawables[BITS].push_back(&b->sprite);
            }
            else if (crops.contains(x) && crops[x].contains(y)) {
                drawables[BITS].push_back(&crops[x][y]);
            }

        }

        if (!flying && y == p.y) {
            flying = true;
            drawables[BITS].push_back(&player);
        }
    }
}

void Game_Renderer::startFade(sf::RectangleShape* fade)
{
    drawables[FADE].push_back(fade);
}

void Game_Renderer::endFade()
{
    drawables[FADE].clear();
}

void Game_Renderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& v : drawables) {
        for (const auto& d : v.second) {
            target.draw(*d, states);
        }
    }
}
