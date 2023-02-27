#pragma once

#include <SFML/Graphics.hpp>

#include <system/state.hpp>

////////////////////////////////////////////////////////////
/// \brief Button class for mouse navigation.
///
class Button : public sf::Drawable {
public:
    Button() = default;
    Button(std::string nlabel, sf::Font& font);
    Button(std::string nlabel, sf::Font& font, std::function<void()> target);


    void update(sf::Vector2f& mpos);

    bool isHighlighted();
    void highlight();
    void unhighlight();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition();

    bool isAvailable();
    void setAvailable();
    void unsetAvailable();

    bool click();

    std::function<void()> target;

protected:
    sf::RectangleShape container;
    sf::Text label;

    bool highlighted = false;
    bool available = true;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    const static float padding;
};

/////////////////////////////////////////////////////////////
/// \brief extension of Button for effecting main state changes
///
struct Nav : public Button {
    Nav() = default;
    Nav(std::string nlabel, sf::Font& font, Main_State ntmain, Menu_State ntmenu);
    Main_State target_main;
    Menu_State target_menu;
};
