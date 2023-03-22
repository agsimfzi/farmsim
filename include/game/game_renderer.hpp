#pragma once
#include <SFML/Graphics.hpp>

#include <map>
#include <vector>

#include <world/world.hpp>

/// GAME RENDERER ///
///
/// \brief Reads all local drawables and layers them for rendering
///
class Game_Renderer : public sf::Drawable {
public:

/// DEFAULT CONSTRUCTOR ///
    Game_Renderer() = default;

/// clear ///
///
/// \brief clears all vectors
    void clear();

/// load ///
///
/// \brief prepares the renderer using local drawables
///
    void load(World& world, Player& player);

private:
    std::vector<std::shared_ptr<Item>> items; /**< intermediate world item storage */

/// placeEntitySprite ///
///
/// \brief places an entity sprite for proper occlusion by details (e.g. trees)
///
    void placeEntitySprite(int y, sf::Sprite& sprite);

    const static sf::Vector2i render_distance; /**< draw distance for tiles */
    sf::Vector2i render_start; /**< minimum tile to be drawn */
    sf::Vector2i render_end; /**< maximum tile to be drawn */

    std::vector<std::vector<sf::Drawable*>> drawables; /**< packaged and layered drawables */

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
