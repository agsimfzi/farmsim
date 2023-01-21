#pragma once

#include <SFML/Graphics.hpp>

#include <audio/volume_type.hpp>

#include <resources/palette.hpp>

////////////////////////////////////////////////////////////
/// \brief Slider for settings menu.
///
class Slider : public sf::Drawable {
public:
    Slider() { }

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    Slider(std::string ntitle);

    ////////////////////////////////////////////////////////////
    /// \brief Draws slider to the render target.
    ///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    ////////////////////////////////////////////////////////////
    /// \brief Prepares the bar.
    ///
    /// \param \b pos -> rects/texts.setPosition()
    /// \param \b font -> texts.setFont()
    ///
    void set(sf::Vector2f pos, sf::Font& font);

    ////////////////////////////////////////////////////////////
    /// \brief Finds the fill by the mouse x position.
    ///
    /// \param \b sf::Mouse::getPosition().x -> setFill()
    ///
    void findFill(int mX); //finds by mousepos

    ////////////////////////////////////////////////////////////
    /// \brief Returns the fill as a percentage out of 100.
    ///
    /// \return \b getFill() * 100.f
    ///
    float getFill() const; //returns 0-100

    ////////////////////////////////////////////////////////////
    /// \brief Changes the value by a fixed value.
    ///
    /// \param \b delta -> setFill()
    ///
    void scroll(float delta);

    ////////////////////////////////////////////////////////////
    /// \brief Checks to see if the mouse is inside the container,
    /// for tracking change and scrolling.
    ///
    /// \return \b true if mouse is inside container
    ///
    bool checkMouse();

    void update();

    bool unclick();

    bool click();

    ////////////////////////////////////////////////////////////
    /// \brief Sets the fill.
    /// Internal.
    ///
    /// \param \b f -> fill.setSize()
    ///
    void setFill(float f);

protected:
    sf::Text title; /**<Title label*/
    sf::Text label; /**<On-container label*/

    sf::RectangleShape container; /**<Holds the slider*/
    const static sf::Vector2f containerSize; /**<Container size*/

    sf::RectangleShape fill; /**<Slider*/
    const static float offset; /**<Distance of slider from container*/
    const static sf::Color fillColor; /**<Slider fill color*/

    const static float scrollFactor; /**<Rate at which scrolling changes the fill*/

    bool changing = false;
};
