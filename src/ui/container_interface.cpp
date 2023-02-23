#include <ui/container_interface.hpp>

Container_Interface::Container_Interface(Player_Inventory& inventory, sf::View& view, Container* container)
    : Inventory_Interface::Inventory_Interface(inventory, view)
    , container{ container }
{
    sf::Vector2f pos = cells[1].front().getPosition();
    pos.y -= (Inventory_Cell::size + cell_padding);
    float sx = pos.x;
    for (const auto& row : container->getInventory()) {
        pos.x = sx;
        pos.y -= (Inventory_Cell::size + cell_padding);
        cells.push_back(std::vector<Inventory_Cell>());
        for (const auto& item : row) {
            cells.back().push_back(Inventory_Cell(item));
            cells.back().back().setPosition(pos);
            pos.x += Inventory_Cell::size + cell_padding;
        }
    }

    pos = cells[1].back().getPosition();

    pos.x += (Inventory_Cell::size * 1.5f);
    pos.y -= (Inventory_Cell::size / 2.f);
    reaction_pos = pos;

    frame.setSize(sf::Vector2f(0.f, 0.f));
}

void Container_Interface::shiftClickLeft()
{
    if (!dragging && mousedItem()) {
        std::shared_ptr<Item> i = std::make_shared<Item>(*mousedItem());
        if (moused.x >= (int)inventory.rowCount) {
            inventory.addItem(i);
            mousedCell()->setItem(i);
            readInventory();
            writeExtension();
        }
        else if (moused.x >= 0) {
            container->addItem(i);
            mousedCell()->setItem(i);
            writeInventory();
            readExtension();
        }
    }
}

void Container_Interface::shiftClickRight()
{
    if (!dragging && mousedItem()) {
        std::shared_ptr<Item> i = std::make_shared<Item>(*mousedItem());
        float intermediate = i->count();
        intermediate /= 2.f;
        intermediate += 0.9f;
        size_t diff = intermediate;
        mousedCell()->take(diff);
        i->setCount(diff);
        if (moused.x >= (int)inventory.rowCount) {
            inventory.addItem(i);
            if (i) {
                mousedCell()->add(i->count());
            }
            readInventory();
            writeExtension();
        }
        else if (moused.x >= 0) {
            container->addItem(i);
            if (i) {
                mousedCell()->add(i->count());
            }
            writeInventory();
            readExtension();
        }
    }
}

void Container_Interface::readExtension()
{
    for (size_t r = inventory.rowCount; r < cells.size(); r++) {
        for (size_t c = 0; c < cells[r].size(); c++) {
            size_t row = r - inventory.rowCount;
            cells[r][c].setItem(container->getItem(sf::Vector2i(row, c)));
        }
    }
}

void Container_Interface::writeExtension()
{
    for (size_t r = inventory.rowCount; r < cells.size(); r++) {
        for (size_t c = 0; c < cells[r].size(); c++) {
            size_t row = r - inventory.rowCount;
            container->setItem(cells[r][c].getItem(), sf::Vector2i(row, c));
        }
    }
}
