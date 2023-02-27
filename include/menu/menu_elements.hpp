#pragma once

#include <memory>

#include <input/input_mapper.hpp>

#include <system/state.hpp>

#include <ui/button.hpp>
#include <ui/slider.hpp>

class Logo : public sf::Drawable {
public:
    Logo() = default;
    Logo(std::string nurl, sf::Vector2f pos, std::string key);

    bool isHighlighted();
    void highlight();
    void unhighlight();
    void update(sf::Vector2f& mpos);

    bool click();

private:
    std::string url;

    sf::Sprite logo;

    sf::Texture texture_logo;

    bool highlighted;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
