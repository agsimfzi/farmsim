#include <ui/scrollable.hpp>

#include <util/vector2_stream.hpp>

void Scrollable::setView(sf::Vector2f pos, sf::Vector2f size, sf::Vector2u window_size)
{
    this->size = size;
    sf::Vector2f wsize(window_size);
    float xs = size.x / wsize.x;
    float ys = size.y / wsize.y;

    float xp = pos.x / wsize.x;
    float yp = pos.y / wsize.y;

    view.setViewport(sf::FloatRect(xp, yp, xs, ys));
    view.setSize(sf::Vector2f(wsize.x * xs, wsize.y * ys));
    view.setCenter(sf::Vector2f(size.x / 2, size.y / 2));
}

sf::View Scrollable::getView()
{
    return view;
}

void Scrollable::reset()
{
    scrollToTop();
    scrollbar.setSize(sf::Vector2f(scrollbar.getSize().x, 0.f));
    frame.height = 0.f;
}

void Scrollable::setScrollable(float max_height)
{
    scrollbar.setFillColor(sf::Color::Black);
    scrollbar.setSize(sf::Vector2f(12.f, 12.f));

    scroll_x_pos = view.getSize().x - scrollbar.getSize().x;

    float fheight = max_height;
    if (size.y > fheight) {
        fheight = size.y;
    }
    frame = sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(size.x, fheight));

    max_scroll = max_height - view.getSize().y;
    if (max_scroll < 0.f) {
        max_scroll = 0.f;
    }

    resizeScrollbar();
    placeScrollbar();
}

bool Scrollable::scroll(float delta, sf::Vector2f mpos){
    bool c = contains(mpos);

    if (c) {
        delta *= -40.f;
        view.move(0.f, delta);
        current_scroll += delta;
        if(current_scroll < 0){
            view.move(0.f, -current_scroll);
            current_scroll = 0;
        }
        else if(current_scroll > max_scroll){
            view.move(0.f, -(current_scroll - max_scroll));
            current_scroll = max_scroll;
        }
        placeScrollbar();
    }

    return c;
}

bool Scrollable::contains(sf::Vector2f mpos)
{
    return frame.contains(mpos);
}

void Scrollable::scrollToTop()
{
    view.move(sf::Vector2f(0.f, -current_scroll));
    current_scroll = 0.f;
}

void Scrollable::resizeScrollbar(){
    sf::Vector2f scroll_size(scrollbar.getSize().x, view.getSize().y);
    if(frame.height > scroll_size.y){
        scroll_size.y /= frame.height;
        scroll_size.y *= size.y;
    }
    scrollbar.setSize(scroll_size);
    placeScrollbar();
}

void Scrollable::placeScrollbar(){
    sf::Vector2f pos(scroll_x_pos, current_scroll);
    pos.y += (size.y - scrollbar.getSize().y) * (current_scroll / max_scroll);
    if(max_scroll > size.y){
        //pos.y += (frame.height - scrollbar.getSize().y) * (current_scroll / max_scroll);
    }
    scrollbar.setPosition(pos);
}
