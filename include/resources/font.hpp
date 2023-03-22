#pragma once

/// FONT ///
///
/// \brief enumerated type to use as keys with the font manager
///
enum class Font {
    MENU,
    UI,
    NULL_FONT
};

/// fontToString ///
///
/// \brief converts passed font enumeration to a string
///
inline std::string fontToString(Font f)
{
    switch (f) {
        case Font::MENU:
            return "MENU";
        case Font::UI:
            return "UI";
        default:
            return "";
    }
}

/// stringToFont
///
/// \brief
/// converts passed string to an enumerated font
inline Font stringToFont(std::string s)
{
    if (s == "MENU") {
        return Font::MENU;
    }
    else if (s == "UI") {
        return Font::UI;
    }
    else {
        return Font::NULL_FONT;
    }
}
