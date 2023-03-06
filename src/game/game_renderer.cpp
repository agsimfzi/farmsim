#include <game/game_renderer.hpp>

void Game_Renderer::clear()
{
    drawables.clear();
    floor.clear();
    details.clear();
    trees.clear();
    rocks.clear();
    buildings.clear();
    items.clear();
}

void Game_Renderer::load(World& world, Player& player)
{
    drawables.clear();
    // 0 floor
    // 1 details
    // 2 world bits
    // 3 extra layer
    drawables.resize(4);

    Chunk_Loader& loader = world.getChunks();
    std::vector<Chunk*> chunks = loader.localChunks();
    size_t n = chunks.size();
    std::vector<std::shared_ptr<Item>> items;
    for (size_t i = 0; i < n; i++) {
        items.insert(items.end(), chunks[i]->getItems().begin(), chunks[i]->getItems().end());
        drawables[3].push_back(&chunks[i]->getFrame());
    }
    Map_Tile<Crop>& crops = world.getCrops();
    std::vector<std::shared_ptr<Vehicle>>& vehicles = world.getVehicles();


    for (const auto& v : vehicles) {
        drawables[1].push_back(v.get());
    }

    for (auto& i : items) {
        drawables[2].push_back(i.get());
    }

    bool flying = (player.getState() == Entity_State::BROOMING);
    if (flying) {
        drawables[3].push_back(&player);
    }

    sf::Vector2i p(player.getCoordinates(Tile::tile_size));
    sf::Vector2i distance(20, 12);
    sf::Vector2i start = p - distance;
    sf::Vector2i end = p + distance;

    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            sf::Vector2i i(x, y);
            Floor* f = loader.floor(i);
            if (f) {
                drawables[0].push_back(f);
            }
            Detail* d = loader.detail(i);
            if (d) {
                drawables[0].push_back(d);
            }

            sf::Sprite* r = loader.rock(i);
            if (r) {
                drawables[2].push_back(r);
            }
            else {
                std::shared_ptr<Building> b = loader.building(i);
                if (b) {
                    drawables[2].push_back(&b->sprite);
                }
                else if (crops.contains(x) && crops[x].contains(y)) {
                    drawables[2].push_back(&crops[x][y]);
                }
            }

        }

        if (!flying && y == p.y) {
            flying = true;
            drawables[2].push_back(&player);
        }

        for (int x = end.x; x >= start.x; x--) {
            Tree* t = loader.tree(sf::Vector2i(x, y));
            if (t) {
                drawables[2].push_back(t);
            }
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
