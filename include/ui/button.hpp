#pragma once

#include <SFML/Graphics.hpp>

#include <system/state.hpp>

////////////////////////////////////////////////////////////
/// \brief Button class for mouse navigation.
///
class Button : public sf::Drawable {
public:
    Button() = default;
    Button(std::string nlabel, sf::Font& font, unsigned int csize);
    Button(std::string nlabel, sf::Font& font, std::function<void()> target, unsigned int csize);


    void update(sf::Vector2f& mpos);

    bool isHighlighted();
    void highlight();
    void unhighlight();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition();

    sf::Vector2f getSize();

    bool isAvailable();
    void setAvailable();
    void unsetAvailable();

    bool click();

    std::function<void()> target;

protected:
    sf::RectangleShape frame;
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
    Nav(std::string nlabel, sf::Font& font, Main_State ntmain, Menu_State ntmenu, unsigned int csize);
    Main_State target_main;
    Menu_State target_menu;
};
