#pragma once

/////////////////////////////////////////////////////////////
/// \brief
///
enum class Font {
    MENU,
    UI,
    NULL_FONT
};

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

inline Font stringToFont(std::string s)
{
    if (s == "MENU") {
        return Font::MENU;
    }
    else if (s == "UI") {
        return Font::UI;
    }
    else
        return Font::NULL_FONT;
}
