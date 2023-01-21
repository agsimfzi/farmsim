#pragma once

#include <SFML/Graphics.hpp>

#include "action.hpp"
#include "convert_action_trigger.hpp"
#include "input_package.hpp"
#include "mouse_event.hpp"

///////////////////////////////////////////////////////////////////////////////////
/// \brief A visual interface for remapping controls.
//
/* PUBLIC METHODS *****************************************************************
 *
 * Input_Mapper()
 *     This default constructor is empty, and is only there for utility (i.e. certain container contexts which require it)
 *
 * void setActions(const sf::Font& font, const std::vector<Action>& actions
 *     Creates rows from action class (name/function/trigger)
 *
 * std::vector<Action> getActions()
 *
 *
 * void reset()
 *     unhovers, unselects, and reverts changes
 *
 * void confirm()
 *     unhovers, unselects, and saves changes
 *
 * void setPosition(sf::Vector2f pos)
 *     places the object - no origin is defined, so this is the top-left corner
 *
 * bool checkMouse(sf::Vector2i mpos)
 *     updates highlight state. returns true if the mouse is in the object.
 *
 * bool clickLeft()
 *     for selecting rows and setting input. returns true if parsed.
 *
 * bool clickRight()
 *     for setting input. returns true if parsed
 *
 * bool keyPressed(sf::Keyboard::Key key)
 *     sets input, if key != Escape. deselects regardless.
 *
 * void setTextColor(const sf::Color& color)
 *     for all row styles
 *
 * void setBackgroundColor(const sf::Color& color)
 *     for regular rows
 *
 * void setHoverColor(const sf::Color& color)
 *     for moused rows
 *
 * void setSelectColor(const sf::Color& color)
 *     for selected rows
 *
 *
 * EXAMPLE ************************************************************************
 *
 * Input_Mapper input_mapper();
 * input_mapper.setPosition(sf::Vector2f(128.f, 128.f));
 * std::vector<Action> actions;
 *     actions.push_back(Action("move north", std::function<void()>(), sf::Keyboard::W));
 *     actions.push_back(Action("move west", std::function<void()>(), sf::Keyboard::A));
 *     actions.push_back(Action("move south", std::function<void()>(), sf::Keyboard::S));
 *     actions.push_back(Action("move east", std::function<void()>(), sf::Keyboard::D));
 *
 * sf::font font;
 * font.loadFromFile(" ... ");
 * input_mapper.setActions(font, actions);
 *
 **********************************************************************************/

class Input_Mapper : public sf::Drawable {
public:
    Input_Mapper() { }

    std::vector<Action> getActions();

    bool keyPressed(sf::Keyboard::Key key);

    void reset();

    void setActions(const sf::Font& font, const std::map<std::string, Action>& actions);

    void confirm();

    void setPosition(sf::Vector2f pos);

    //recommend running checkMouse() every frame, for post-selection hovers
    bool checkMouse(sf::Vector2f mpos);

    bool clickLeft();
    bool clickRight();

    void setTextColor(const sf::Color& color);
    void setBackgroundColor(const sf::Color& color);
    void setHoverColor(const sf::Color& color);
    void setSelectColor(const sf::Color& color);

protected:
    class Row;

    Convert_Action_Trigger converter;

    sf::Vector2f pos { 0.f, 0.f };
    sf::Vector2f rowSize { 512.f, 72.f };

    size_t hoverIndex { SIZE_MAX };
    size_t selectIndex { SIZE_MAX };

    void checkPrev(const std::string& k);

    std::vector<Row> rows;

    unsigned int characterSize = 36;

    void undoHover();
    void undoSelect();

    void addAction(const sf::Font& font, Action action, sf::Vector2f& rpos);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Input_Mapper::Row : public sf::Drawable, public Action {
public:
    Row(const Action& action, const sf::Font& font, unsigned int characterSize, sf::Vector2f size);

    enum State {
        NONE,
        HOVER,
        SELECT
    };

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);

    const sf::Vector2f getSize() const;

    const static int padding = 6;

    bool checkMouse(sf::Vector2f mpos);

    void reset();
    void setKey(std::string key);
    void confirm();

    void resetColors();
    void setState(Row::State state);
    void setCellColor(const Row::State state, const sf::Color& color);
    void setTextColor(const sf::Color& color);

    std::string getLabel();
    std::string getKey();
    std::function<void()> press;
    std::function<void()> release;

    Action asAction();

private:
    std::pair<sf::RectangleShape, sf::RectangleShape> cells;
    std::pair<sf::Text, sf::Text> labels;

    std::string cache; //for resets

    sf::FloatRect bounds;

    State state; //must be initialized through setState() in constructor

    static std::map<State, sf::Color> colors;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
