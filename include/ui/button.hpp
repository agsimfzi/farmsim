#pragma once

#include <SFML/Graphics.hpp>

#include <system/state.hpp>

/// BUTTON ///
///
/// \brief Button class for mouse navigation.
///
class Button : public sf::Drawable {
public:
    Button() = default;

/// FULL CONSTRUCTOR ///
///
/// \brief constructs the button, without a target
///
    Button(std::string nlabel, sf::Font& font, unsigned int csize);

/// FULL CONSTRUCTOR WITH TARGET ///
///
/// \brief constructs the button along with an onclick callback
///
    Button(std::string nlabel, sf::Font& font, std::function<void()> target, unsigned int csize);

/// update ///
///
/// \brief checks mouse highlight
///
    void update(sf::Vector2f& mpos);

/// isHighlighted ///
///
/// \brief returns true if the button is moused and highlighted
///
    bool isHighlighted();

/// highlight ///
///
/// \brief sets highlighted to true and changes colors accordingly
///
    void highlight();

/// unhighlight ///
///
/// \brief sets highlighted to false and changes colors accordingly
///
    void unhighlight();

/// setPosition ///
///
/// \brief places the frame and text
///
    void setPosition(sf::Vector2f pos);

/// getPosition ///
///
/// \brief returns the frame's position (centered)
///
    sf::Vector2f getPosition();

/// getSize ///
///
/// \brief returns the frame's size
///
    sf::Vector2f getSize();

/// isAvailable ///
///
/// \brief returns true if click is enabled
///
    bool isAvailable();

/// setAvailable ///
///
/// \brief enables click
///
    void setAvailable();

/// unsetAvailable ///
///
/// \brief disables click
///
    void unsetAvailable();

/// click ///
///
/// \brief returns true if button is available and highlighted. also calls target if applicable
///
    bool click();

    std::function<void()> target; /**< onclick callback */

protected:
    sf::RectangleShape frame; /**< button frame */
    sf::Text label; /**< button text */

    bool highlighted = false; /**< tracks mouseover */
    bool available = true; /**< tracks whether button may be clicked */

/// draw ///
///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    const static float padding; /**< space to be made in the frame around the label */
};

/// NAV ///
///
/// \brief extension of Button for effecting main state changes
///
struct Nav : public Button {
    Nav() = default;

/// FULL CONSTRUCTOR ///
///
/// \brief sets the state targets and defines the label
    Nav(std::string nlabel, sf::Font& font, Main_State ntmain, Menu_State ntmenu, unsigned int csize);
    Main_State target_main; /**< onclick main state target */
    Menu_State target_menu; /**< onclick menu state target */
};
