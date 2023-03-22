#pragma once

#include <SFML/Graphics.hpp>

#include <string>

/// LOGO ///
///
/// \brief for drawing logos in menus. URL target on click.
///
class Logo : public sf::Drawable {
public:
    Logo() = default;

/// FULL CONSTRUCTOR ///
///
/// \brief loads the url and places the logo
///
/// \param key Texture key
///
    Logo(std::string nurl, sf::Vector2f pos, std::string key);

/// isHighlighted ///
///
/// \brief returns true if mouse highlight is active
///
    bool isHighlighted();

/// highlight ///
///
/// \brief activate mouse highlight
///
    void highlight();

/// unhighlight ///
///
/// \brief deactivate mouse highlight
///
    void unhighlight();

/// update ///
///
/// \brief checks highlight
///
    void update(sf::Vector2f& mpos);

/// click ///
///
/// \brief launches URL if highlighted
///
/// \return true if highlighted
///
    bool click();

private:
    std::string url; /**< website URL target */

    sf::Sprite logo;

    bool highlighted = false; /**< mouse highlight state */

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
