#pragma once

#include <SFML/Graphics/Font.hpp>

#include <resources/font.hpp>

/// FONT MANAGER ///
///
/// \brief singleton object that stores font objects and provides references to them
///
class Font_Manager {
public:

/// DEFAULT DESTRUCTOR ///
///
/// \brief deletes the instance
///
    ~Font_Manager();

/// get ///
///
/// \brief returns a reference to the font keyed to the passed enumerated type
///
    static sf::Font& get(Font f);

private:
    static std::map<Font, sf::Font> fonts; /**< font storage */

/// DEFAULT CONSTRUCTOR ///
///
/// \brief loads fonts from the database. private to enforce singleton behavior
///
    Font_Manager();

    static Font_Manager* instance; /**< singleton instance */
};
