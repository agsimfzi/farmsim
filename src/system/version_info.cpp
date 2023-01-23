#include <system/version_info.hpp>

#include <resources/font_manager.hpp>

#include "version.h"

Version_Info::Version_Info()
    : sf::Text("", Font_Manager::get(Font::UI), 36);
{
    std::string v = AutoVersion::STATUS;
    setString(v);
    setPosition(sf::Vector2f(1600.f, 24.f));
    setFillColor(sf::Color(200, 200, 200));
}
