#include <game/game_renderer.hpp>

const sf::Vector2i Game_Renderer::renderDistance { 24, 16 };

void Game_Renderer::clear()
{
    base.clear();
    layered.clear();
}

void Game_Renderer::updatePlayerCoordinates(sf::Vector2i coordinates)
{
    playerCoordinates = coordinates;
    renderStart = playerCoordinates - renderDistance;
    renderEnd = playerCoordinates + renderDistance;
}

void Game_Renderer::loadFloor(Map_Tile<Floor>& floor)
{
    for (int x = renderStart.x; x <= renderEnd.x; x++) {
        for (int y = renderStart.y; y <= renderEnd.y; y++) {
            if (floor.contains(x) && floor[x].contains(y) && floor[x][y]) {
                base.push_back(*floor[x][y]);
                for (const auto& d : floor[x][y]->details) {
                    base.push_back(d);
                }
            }
        }
    }
}

void Game_Renderer::loadCrops(Map_Tile<Crop>& crops)
{
    for (int x = renderStart.x; x <= renderEnd.x; x++) {
        for (int y = renderStart.y; y <= renderEnd.y; y++) {
            if (crops.contains(x) && crops[x].contains(y) && crops[x][y]) {
                layered[y].push_back(crops[x][y]->getSprite());
            }
        }
    }
}

void Game_Renderer::loadWalls(Map_Tile<Wall>& walls)
{
    for (int x = renderStart.x; x <= renderEnd.x; x++) {
        for (int y = renderStart.y; y <= renderEnd.y; y++) {
            if (walls.contains(x) && walls[x].contains(y) && walls[x][y]) {
                layered[y].push_back(*walls[x][y]);
            }
        }
    }
}

void Game_Renderer::loadPlayer(Player& player)
{
    placeEntitySprite(playerCoordinates.y, player.getSprite());
}

void Game_Renderer::loadEntities(std::vector<Entity>& entities)
{
    for (auto& e : entities) {
        placeEntitySprite(e.getCoordinates(Tile::tileSize).y, e.getSprite());
    }
}

void Game_Renderer::placeEntitySprite(int y, sf::Sprite& sprite)
{
    if ((sprite.getPosition().y / Tile::tileSize) < y) {
        y -= 1;
    }
    layered[y].push_back(sprite);
}

void Game_Renderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& s : base) {
        target.draw(s, states);
    }

    for (const auto& v : layered) {
        for (const auto& sprite : v.second) {
            target.draw(sprite, states);
        }
    }
}
