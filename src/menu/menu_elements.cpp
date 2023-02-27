#include <menu/menu_elements.hpp>

#include <resources/texture_manager.hpp>

#include <util/url.hpp>

//////////////////////////////////////////////////////////////

Logo::Logo(std::string nurl, sf::Vector2f pos, std::string key)
    : url { nurl }
{
    sf::Vector2f size = sf::Vector2f((logo.getGlobalBounds().left + logo.getGlobalBounds().width), (logo.getGlobalBounds().top + logo.getGlobalBounds().height));

    logo.setOrigin(size / 2.f);
    logo.setPosition(pos);
}

void Logo::update(sf::Vector2f& mpos)
{
    bool contains = logo.getGlobalBounds().contains(mpos);
    if (!highlighted && contains) {
        highlight();
    }
    else if (highlighted && !contains) {
        unhighlight();
    }
}

bool Logo::click()
{
    if (highlighted) {
        launchURL(url);
    }

    return highlighted;
}

void Logo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(logo, states);
}

bool Logo::isHighlighted()
{
    return highlighted;
}

void Logo::highlight()
{
    highlighted = true;
}

void Logo::unhighlight()
{
    highlighted = false;
}
