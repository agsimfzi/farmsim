#include <ui/container_interface.hpp>

#include <util/vector2_stream.hpp>

Container_Interface::Container_Interface(Player_Inventory& inventory, sf::View& view, Container* container)
    : Inventory_Interface::Inventory_Interface(inventory, view)
    , container{ container }
    , c_inventory{ container->getInventory() }
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

    pos = cells.back().back().getPosition();

    pos.x += (Inventory_Cell::size * 1.5f);
    pos.y -= (Inventory_Cell::size / 2.f);

    sf::Vector2f size(320.f, 512.f);
    reaction_interface.setView(pos, size);

    frame.setSize(sf::Vector2f(0.f, 0.f));
}

void Container_Interface::shiftClickLeft()
{}

void Container_Interface::shiftClickRight()
{}

void Container_Interface::readExtension()
{
    for (size_t r = 0; r < c_inventory.size(); r++) {
        for (size_t c = 0; r < c_inventory[r].size(); c++) {
            size_t row = r + inventory.rowCount;
            cells[row][c].setItem(c_inventory[r][c]);
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
