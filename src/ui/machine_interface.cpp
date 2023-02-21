#include <ui/machine_interface.hpp>

#include <util/fmouse.hpp>

Machine_Interface::Machine_Interface(Player_Inventory& inventory, sf::View& view, Machine* machine)
    : Inventory_Interface(inventory, view)
    , machine{ machine }
{
    setProgressBarSize();

    std::vector<std::vector<std::shared_ptr<Item>>>& building_inventory = machine->getInventory();

    sf::Vector2f pos = cells[1].back().getPosition();
    pos.x += (Inventory_Cell::size * 2.f);
    float sx = pos.x;
    sf::Vector2f progress_pos = pos;
    progress_pos.x += ((building_inventory.front().size() - 1) * Inventory_Cell::size) / 2.f;
    progress_pos.y += Inventory_Cell::size;
    progress_bar.setPosition(progress_pos);
    for (const auto& row : building_inventory) {
        pos.x = sx;
        cells.push_back(std::vector<Inventory_Cell>());
        for (const auto& item : row) {
            cells.back().push_back(Inventory_Cell(item));
            cells.back().back().setPosition(pos);
            pos.x += Inventory_Cell::size;
        }

        pos.y += Inventory_Cell::size * 2.f;
    }
    progress_pos.y += Inventory_Cell::size;

    cells.back().back().setPosition(progress_pos);

    pos = cells[inventory.rowCount].back().getPosition();
    pos.x += Inventory_Cell::size;
    pos.y -= Inventory_Cell::size;
    pos.y -= 256.f;

    sf::Vector2f size(320.f, 512.f);
    reaction_interface.setView(pos, size);

    pos = cells[inventory.rowCount].front().getPosition();
    pos.x -= Inventory_Cell::size;
    pos.y -= Inventory_Cell::size;

    size.x = cells[inventory.rowCount].back().getPosition().x + Inventory_Cell::size;
    size.y = cells.back().back().getPosition().y + Inventory_Cell::size;
    size -= pos;

    frame.setPosition(pos);
    frame.setSize(size);
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
                cells[moused_index.x][moused_index.y].updateCount();
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
        else {
            dragging = true;
        }
    }
    else if (moused_index.x == rows + 1) {
        cancelDrag();
    }
    else if (moused_index.x < rows) {
        Inventory_Interface::placeMergeSwap();
        writeExtension();
    }
}

void Machine_Interface::readExtension()
{
    std::vector<std::shared_ptr<Item>> reagants = machine->getInventory().front();
    for (size_t i = 0; i < reagants.size(); i++) {
        cells[inventory.rowCount][i].clearItem();
        cells[inventory.rowCount][i].setItem(reagants[i]);
    }

    cells.back().front().clearItem();
    cells.back().front().setItem(machine->getInventory().back().front());
    reaction_interface.check(inventory);
}

void Machine_Interface::writeExtension()
{
    size_t r = inventory.rowCount;
    size_t n = cells[r].size();
    for (size_t i = 0; i < n; i++) {
        std::shared_ptr<Item> item = cells[r][i].getItem();
        if (item) {
            machine->setReagant(item, i);
        }
        else {
            machine->clearReagant(i);
        }
    }
    machine->setProduct(cells.back().front().getItem());
    machine->checkReaction();
    reaction_interface.check(inventory);
}

void Machine_Interface::swap()
{
    Inventory_Interface::swap();

    int rows = inventory.rowCount;

    if (dragStartIndex.x == rows || moused_index.x == rows) {
        machine->clearReagant(dragStartIndex.y);
        machine->setReagant(cells[dragStartIndex.x][dragStartIndex.y].getItem(), moused_index.y);
        writeExtension();
    }
}

void Machine_Interface::startDrag()
{
    Inventory_Interface::startDrag();
    if (moused_index.x == (int)inventory.rowCount) {
        machine->clearReagant(moused_index.y);
        machine->checkReaction();
        setProgressBarSize();
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

        if (reaction) {
            // move reagants in machine to inventory
            for (auto& c : cells[inventory.rowCount]) {
                std::shared_ptr<Item> i = c.getItem();
                if (i) {
                    inventory.addItem(i);
                    if (i) {
                        // ends the process if there's no remaining inventory space
                        return;
                    }
                    else {
                        c.clearItem();
                    }
                }
            }
            writeExtension();

            // move new reagants to machine
            for (auto& r : reaction->reagants) {
                std::shared_ptr<Item> i = inventory.findItem(r.name);
                if (i) {
                    machine->addReagant(i);
                }
            }
            // re-read the interface
            readInventory();
            readExtension();
            reaction_interface.check(inventory);
        }
    }
    else {
        Inventory_Interface::clickLeft(window);
        reaction_interface.check(inventory);
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
    size.x = Inventory_Cell::size;
    size.y = (Inventory_Cell::size * machine->reactionProgress());
    progress_bar.setSize(size);
}
