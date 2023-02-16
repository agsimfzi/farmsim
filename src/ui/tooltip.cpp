#include <ui/tooltip.hpp>

#include <resources/font_manager.hpp>

#include <util/primordial.hpp>

Tooltip::Tooltip(std::shared_ptr<Item> item)
{
    name.setString(item->getName());
    name.setFont(Font_Manager::get(Font::UI));
    name.setFillColor(sf::Color::Black);
    name.setCharacterSize(32);

    description.setString(item->getDescription());
    description.setFont(Font_Manager::get(Font::UI));
    description.setFillColor(sf::Color::Black);
    description.setCharacterSize(24);

    int v = item->getValue();
    std::string vstring = "Value: ";
    if (v >= 0) {
        vstring += std::to_string(v);
    }
    else {
        vstring += "N/A";
    }

    value.setString(vstring);
    value.setFont(Font_Manager::get(Font::UI));
    value.setFillColor(sf::Color::Black);
    value.setCharacterSize(24);
}

Tooltip::Tooltip(Reaction& reaction, std::vector<std::shared_ptr<Item>> reagants)
{
    name.setString(reaction.name);
    name.setFont(Font_Manager::get(Font::UI));
    name.setFillColor(sf::Color::Black);
    name.setCharacterSize(32);

    for (auto& i : reagants) {
        sf::Sprite sprite = i->getSprite();
        std::string lstr = "x";
        for (auto& r : reaction.reagants) {
            if (equalStrings(r.name, i->getName())) {
                lstr += std::to_string(r.count);
            }
        }
        sf::Text label;
        label.setString(lstr);
        label.setFont(Font_Manager::get(Font::UI));
        label.setFillColor(sf::Color::Black);
        label.setCharacterSize(20);

        labeled_sprites.push_back(std::make_pair(sprite, label));
    }
}

void Tooltip::setPosition(sf::Vector2f pos)
{
    frame.setPosition(pos);
    name.setPosition(pos);
    description.setPosition(pos);
    for (auto& ls : labeled_sprites) {
        ls.first.setPosition(pos);
        ls.second.setPosition(pos);
    }
}

void Tooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    target.draw(name, states);
    target.draw(description, states);
    target.draw(value, states);
    for (const auto& ls : labeled_sprites) {
        target.draw(ls.first, states);
        target.draw(ls.second, states);
    }
}
