#include <resources/font_manager.hpp>

#include <system/database.hpp>

//////////////////////////////////////////////////////////////

std::map<Font, sf::Font> Font_Manager::fonts = std::map<Font, sf::Font>();

Font_Manager* Font_Manager::instance = nullptr;

Font_Manager::Font_Manager()
{
    Database::getFonts(fonts);
}

Font_Manager::~Font_Manager()
{
    delete instance;
}

sf::Font& Font_Manager::get(Font f)
{
    if (instance == nullptr) {
        instance = new Font_Manager;
    }

    return fonts[f];
}
