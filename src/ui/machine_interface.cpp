#include <ui/machine_interface.hpp>

#include <util/fmouse.hpp>

#include <world/tile.hpp> // for tileSize... make an inventory_cell_size ffs

#include <util/vector2_stream.hpp>

Machine_Interface::Machine_Interface(Player_Inventory& inventory, sf::View& view, Machine* machine)
    : Inventory_Interface(inventory, view)
    , machine{ machine }
{
    if (machine->current_reaction >= 0) {
        setProgressBarSize();
    }

    std::vector<std::vector<std::shared_ptr<Item>>>& building_inventory = machine->getInventory();

    sf::Vector2f pos = cells[1].back().getPosition();
    pos.x += (Tile::tileSize * 2.f);
    float sx = pos.x;
    sf::Vector2f progress_pos = pos;
    progress_pos.y += (Tile::tileSize);
    progress_bar.setPosition(progress_pos);
    for (const auto& row : building_inventory) {
        pos.x = sx;
        cells.push_back(std::vector<Inventory_Cell>());
        for (const auto& item : row) {
            std::cout << "placing cell at " << pos << '\n';
            cells.back().push_back(Inventory_Cell(item));
            cells.back().back().setPosition(pos);
            pos.x += Tile::tileSize;
        }

        pos.y += Tile::tileSize * 2.f;
    }
}

void Machine_Interface::update(sf::RenderWindow& window)
{
    Inventory_Interface::update(window);

    if (machine->current_reaction >= 0) {
        setProgressBarSize();
        checkReaction();
    }
}

void Machine_Interface::placeMergeSwap()
{
    //ugh
    std::shared_ptr<Item> si = cells[moused_index.x][moused_index.y].getItem();
    int rows = inventory.rowCount;
    if (moused_index.x == rows) {
        if (machine->validReagant(dragItem->getName())) {
            if (!si) {
                cells[moused_index.x][moused_index.y].setItem(dragItem);
            }
            else if (dragItem->getUID() == si->getUID()) {
                size_t remainder = si->add(dragItem->count());
                if (remainder == 0) {
                    dragItem = nullptr;
                }
                else {
                    dragItem->setCount(remainder);
                }
            }
            else {
                swap();
            }
            writeExtension();
        }
    }
    else if (moused_index.x < rows) {
        Inventory_Interface::placeMergeSwap();
    }
}

void Machine_Interface::readExtension()
{
}

void Machine_Interface::writeExtension()
{
    size_t r = inventory.rowCount;
    size_t n = cells[r].size();
    for (size_t i = 0; i < n; i++) {
        machine->clearReagant(i);
        machine->addReagant(cells[r][i].getItem());
    }

    machine->clearProduct();
    machine->setProduct(cells.back().front().getItem());
}

void Machine_Interface::swap()
{
    Inventory_Interface::swap();

    int rows = inventory.rowCount;

    if (dragStartIndex.x == rows || moused_index.x == rows) {
        machine->clearReagant(dragStartIndex.y);
        machine->addReagant(cells[dragStartIndex.x][dragStartIndex.y].getItem());
        inventory.placeItem(dragStartIndex.x, dragStartIndex.y, cells[dragStartIndex.x][dragStartIndex.y].getItem());
    }
}

void Machine_Interface::startDrag()
{
    Inventory_Interface::startDrag();
    if (moused_index.x == (int)inventory.rowCount) {
        machine->clearReagant(moused_index.y);
    }
    else if (moused_index.x == (int)inventory.rowCount + 1) {
        machine->clearProduct();
    }
}

void Machine_Interface::cancelDrag()
{
    cells[dragStartIndex.x][dragStartIndex.y].setItem(dragItem);

    if (dragStartIndex.x == (int)inventory.rowCount) {
        machine->addReagant(dragItem);
        if (dragItem) {
            dragging = true;
            return;
        }
    }
    else if (dragStartIndex.x == (int)inventory.rowCount + 1) {
        machine->setProduct(dragItem);
    }
    dragging = false;
    dragItem.reset();
}

void Machine_Interface::clickLeft(sf::RenderWindow& window)
{
    sf::Vector2f mpos = fMouse(window, reaction_interface.getView());
    if (reaction_interface.contains(mpos)) {
        std::pair<Reaction*, std::shared_ptr<Item>> rxn = reaction_interface.click(fMouse(window, reaction_interface.getView()));
        Reaction* reaction = rxn.first;
        // move reagants in machine to inventory
        // move new reagants to machine
        // re-read the interface
    }
    else {
        Inventory_Interface::clickLeft(window);
    }
}

void Machine_Interface::checkReaction()
{
    size_t r = inventory.rowCount;
    size_t n = cells[r].size();
    std::vector<std::shared_ptr<Item>> reagants = machine->activeReagants();
    for (size_t i = 0; i < n; i++) {
        cells[r][i].updateCount();
    }
    r++;
    if (!cells[r].front().getItem()) {
        cells[r].front().setItem(machine->activeProduct());
    }
    else {
        cells[inventory.rowCount + 1].front().updateCount();
    }
}

void Machine_Interface::setProgressBarSize()
{
    sf::Vector2f size;
    size.x = Tile::tileSize;
    size.y = (Tile::tileSize * machine->reactionProgress());
    progress_bar.setSize(size);
}
