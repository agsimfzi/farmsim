#include <menu/menu_elements.hpp>

#include <resources/texture_manager.hpp>

#include <util/url.hpp>

//////////////////////////////////////////////////////////////

Nav::Nav(std::string nlabel, sf::Font& font, Main_State ntmain, Menu_State ntmenu)
    : Button { nlabel, font }
{
    target_main = ntmain;
    target_menu = ntmenu;
}

Target::Target(std::function<void()> nt)
    : target { nt }
{
}

Option::Option(std::string nlabel, sf::Font& font, std::function<void()> nt)
    : Button { nlabel, font }
    , Target(nt)
{
}

Logo::Logo(std::string nurl, sf::Vector2f pos, std::string key)
    : url { nurl }
{
    shadow = std::make_unique<sf::Shader>();
    shadow->loadFromFile("src\\shader\\dropshadow.frag", sf::Shader::Fragment);

    logo.setTexture(Texture_Manager::get(key));
    dropShadow.setTexture(Texture_Manager::get(std::string(key)));

    sf::Vector2f size = sf::Vector2f((logo.getGlobalBounds().left + logo.getGlobalBounds().width), (logo.getGlobalBounds().top + logo.getGlobalBounds().height));

    logo.setOrigin(size / 2.f);
    logo.setPosition(pos);
    dropShadow.setOrigin(size / 2.f);
    dropShadow.setPosition(pos);

    //shadow->setUniform("offsetFactor", sf::Vector2f(0.003f, 0.003f));
    //shadow->setUniform("u_resolution", size);
    //shadow->setUniform("u_pos", pos);
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
    if (highlighted) {
        target.draw(dropShadow, shadow.get());
    }

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
