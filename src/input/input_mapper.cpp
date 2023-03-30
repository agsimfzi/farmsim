#include <input/input_mapper.hpp>

#include <util/primordial.hpp>

#include <resources/palette.hpp>

//////////////////////////////////////////////////////////////
void Input_Mapper::reset()
{

    for (auto& row : rows) {
        row.reset();
    }

    hoverIndex = SIZE_MAX;
    selectIndex = SIZE_MAX;
}

void Input_Mapper::confirm()
{
    for (auto& row : rows) {
        row.confirm();
    }
}

void Input_Mapper::setPosition(sf::Vector2f pos)
{
    this->pos = pos;
    for (auto& row : rows) {
        row.setPosition(pos);
        pos.y += row.getSize().y + 4;
    }
}

bool Input_Mapper::checkMouse(sf::Vector2f mpos)
{
    const size_t n = rows.size();
    for (size_t r = 0; r < n; r++) {
        if (rows[r].checkMouse(mpos)) {
            if (r != hoverIndex && r != selectIndex) {
                undoHover();
                rows[r].setState(Row::HOVER);
                hoverIndex = r;
            }
            return true; //skips the final undoHover() check
        }
    }
    undoHover();
    return false;
}

void Input_Mapper::undoHover()
{
    if (hoverIndex != SIZE_MAX && hoverIndex != selectIndex) {
        rows[hoverIndex].setState(Row::NONE);
        hoverIndex = SIZE_MAX;
    }
}

void Input_Mapper::undoSelect()
{
    if (selectIndex != SIZE_MAX) {
        rows[selectIndex].setState(static_cast<Row::State>(hoverIndex == selectIndex));
        selectIndex = SIZE_MAX;
    }
}

bool Input_Mapper::clickLeft()
{
    if (selectIndex != SIZE_MAX) {
        //set selection as left click
        std::string k = "LMouse";
        checkPrev(k);
        rows[selectIndex].setKey(k);
        undoSelect();
        return true;
    }

    bool parsed = (hoverIndex != SIZE_MAX && hoverIndex != selectIndex);

    if (parsed) {
        undoSelect();
        selectIndex = hoverIndex;
        rows[selectIndex].setState(Row::SELECT);
    }

    return parsed;
}

bool Input_Mapper::clickRight()
{
    bool parsed = (selectIndex != SIZE_MAX);

    if (parsed) {
        std::string k = "RMouse";
        checkPrev(k);
        rows[selectIndex].setKey(k);
        undoSelect();
    }

    return parsed;
}

void Input_Mapper::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& row : rows) {
        target.draw(row, states);
    }
}

bool Input_Mapper::keyPressed(sf::Keyboard::Key key)
{
    bool parsed = (selectIndex != SIZE_MAX);

    if (parsed) {
        if (key != sf::Keyboard::Escape) {
            std::string k = converter.toString(key);
            checkPrev(k);
            rows[selectIndex].setKey(k);
        }

        undoSelect();
    }

    return parsed;
}

void Input_Mapper::checkPrev(const std::string& k)
{
    auto isPrev = [k](Row r) { return r.getKey() == k; };
    auto prevIt = std::find_if(rows.begin(), rows.end(), isPrev);
    if (prevIt != rows.end()) {
        std::string prev = rows[selectIndex].getKey();
        prevIt->setKey(prev);
    }
}

void Input_Mapper::setBackgroundColor(const sf::Color& color)
{
    for (auto& row : rows) {
        row.setCellColor(Row::NONE, color);
    }
}

void Input_Mapper::setHoverColor(const sf::Color& color)
{
    for (auto& row : rows) {
        row.setCellColor(Row::HOVER, color);
    }
}

void Input_Mapper::setSelectColor(const sf::Color& color)
{
    for (auto& row : rows) {
        row.setCellColor(Row::SELECT, color);
    }
}

void Input_Mapper::setTextColor(const sf::Color& color)
{
    for (auto& row : rows) {
        row.setTextColor(color);
    }
}

void Input_Mapper::setActions(const sf::Font& font, const std::map<std::string, Action>& actions)
{
    rows.clear();
    sf::Vector2f rpos = pos;

    static std::vector<std::string> order = { "Move North"
                                            , "Move West"
                                            , "Move South"
                                            , "Move East"
                                            , "Open Inventory"
                                            , "Open Map"
                                            , "Pickup All"
                                            , "Open Building"
                                            , "Spell Menu" };

    size_t n = order.size();

    for (size_t i = 0; i < n; ++i) {
        addAction(font, actions.at(order[i]), rpos);
    }
}

void Input_Mapper::addAction(const sf::Font& font, Action action, sf::Vector2f& rpos)
{
    action.trigger = converter.toString(action.trigger);
    rows.push_back(Row(action, font, characterSize, rowSize));
    rows.back().press = action.press;
    rows.back().release = action.release;
    rows.back().setPosition(rpos);
    rpos.y += rows.back().getSize().y + Row::padding;
}

std::vector<Action> Input_Mapper::getActions()
{
    std::vector<Action> actions;

    for (const auto& row : rows) {
        actions.push_back(row);
        actions.back().trigger = converter.toKey(std::get<std::string>(actions.back().trigger));
    }

    return actions;
}

//////////////////////////////////////////////////////////////
std::map<Input_Mapper::Row::State, sf::Color> Input_Mapper::Row::colors = {
    { Row::NONE, Palette::gray_dark },
    { Row::HOVER, Palette::green },
    { Row::SELECT, Palette::blue } };

Input_Mapper::Row::Row(const Action& action, const sf::Font& font, unsigned int characterSize, sf::Vector2f size)
    : Action(action)
{
    cache = std::get<std::string>(action.trigger);
    labels.first.setFont(font);
    labels.first.setCharacterSize(characterSize);
    labels.first.setString(name);
    centerText(labels.first);

    cells.first.setSize(size - sf::Vector2f(2.f * size.y + padding, 0.f));

    labels.second.setFont(font);
    labels.second.setCharacterSize(characterSize);
    labels.second.setString(std::get<std::string>(action.trigger));
    centerText(labels.second);

    cells.second.setSize(sf::Vector2f(2.f * size.y, size.y));

    bounds.width = size.x;
    bounds.height = size.y;

    setState(Row::NONE);
}

void Input_Mapper::Row::setPosition(sf::Vector2f pos)
{
    bounds.left = pos.x;
    bounds.top = pos.y;

    cells.first.setPosition(pos);
    labels.first.setPosition(cells.first.getPosition() + (cells.first.getSize() / 2.f));
    pos.x += cells.first.getSize().x + padding;
    cells.second.setPosition(pos);
    labels.second.setPosition(cells.second.getPosition() + (cells.second.getSize() / 2.f));
}

sf::Vector2f Input_Mapper::Row::getPosition()
{
    return cells.first.getPosition();
}

bool Input_Mapper::Row::checkMouse(sf::Vector2f mpos)
{
    return (bounds.contains(mpos.x, mpos.y));
}

void Input_Mapper::Row::setState(Input_Mapper::Row::State state)
{
    this->state = state;
    cells.first.setFillColor(colors[state]);
    cells.second.setFillColor(colors[state]);
}

void Input_Mapper::Row::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(cells.first, states);
    target.draw(cells.second, states);

    target.draw(labels.first, states);
    target.draw(labels.second, states);
}

const sf::Vector2f Input_Mapper::Row::getSize() const
{
    return cells.first.getSize() + sf::Vector2f(cells.second.getSize().x, 0.f);
}

void Input_Mapper::Row::setKey(std::string key)
{
    cache = key;
    labels.second.setString(key);
    centerText(labels.second);
}

std::string Input_Mapper::Row::getKey()
{
    return cache;
}

void Input_Mapper::Row::reset()
{
    setKey(std::get<std::string>(trigger));
    setState(Row::NONE);
}

void Input_Mapper::Row::confirm()
{
    trigger.emplace<std::string>(cache);
    setState(Row::NONE);
}

void Input_Mapper::Row::setCellColor(const Row::State state, const sf::Color& color)
{
    colors[state] = color;
    setState(this->state);
}

void Input_Mapper::Row::setTextColor(const sf::Color& color)
{
    labels.first.setFillColor(color);
    labels.second.setFillColor(color);
}

float Input_Mapper::Row::width()
{
    return (cells.second.getPosition().x + cells.second.getSize().x - cells.first.getPosition().x);
}
