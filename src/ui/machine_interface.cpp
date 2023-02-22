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
    progress_arrow.setPosition(progress_pos);
    progress_arrow.setSize(sf::Vector2f(Inventory_Cell::size, Inventory_Cell::size));
    progress_arrow.setOrigin(Inventory_Cell::size / 2.f, Inventory_Cell::size / 2.f);
    for (const auto& row : building_inventory) {
        pos.x = sx;
        cells.push_back(std::vector<Inventory_Cell>());
        for (const auto& item : row) {
            cells.back().push_back(Inventory_Cell(item));
            cells.back().back().setPosition(pos);
            pos.x += Inventory_Cell::size + cell_padding;
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

    readExtension();
}

void Machine_Interface::update(sf::RenderWindow& window)
{
    mousedIndex();
    checkDrag();
    checkTooltip(window);
    checkReaction();
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
/*
void Machine_Interface::swap()
{
    Inventory_Interface::swap();

    int rows = inventory.rowCount;

    if (dragStartIndex.x == rows || moused.x == rows) {
        machine->clearReagant(dragStartIndex.y);
        machine->setReagant(cells[dragStartIndex.x][dragStartIndex.y].getItem(), moused.y);
        writeExtension();
    }
}
*/
bool Machine_Interface::checkReactionInterface(sf::RenderWindow& window)
{
    sf::Vector2f mpos = fMouse(window, reaction_interface.getView());
    bool parsed = (reaction_interface.contains(mpos));
    if (parsed) {
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
                        return parsed;
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
    return parsed;
}

void Machine_Interface::checkReaction()
{
    size_t r = inventory.rowCount;
    size_t n = cells[r].size();
    std::vector<std::shared_ptr<Item>> reagants = machine->activeReagants();
    for (size_t i = 0; i < n; i++) {
        cells[r][i].updateCount();
    }
    r++; // shift to product row
    if (!cells[r].front().getItem()) {
        cells[r].front().setItem(machine->activeProduct());
    }
    else {
        cells[inventory.rowCount + 1].front().updateCount();
    }
    setProgressBarSize();
}

void Machine_Interface::setProgressBarSize()
{
    sf::Vector2f size;
    size.x = Inventory_Cell::size;
    size.y = (Inventory_Cell::size * machine->reactionProgress());
    progress_bar.setSize(size);
}

void Machine_Interface::shiftClickLeft()
{
    std::shared_ptr<Item> i = mousedItem();
    if (i) {
        if (moused.x >= (int)inventory.rowCount) {
            // attempt move whole stack to inventory
            inventory.addItem(i);
            mousedCell()->setItem(i);
            //readInventory();
            writeExtension();
        }
        else if (moused.x >= 0) {
            // attempt move whole stack to machine
            size_t remainder = machine->addReagant(i);
            if (remainder > 0) {
                i->setCount(remainder);
            }
            else {
                mousedCell()->clearItem();
            }
            writeInventory();
            readExtension();
        }
    }
}

void Machine_Interface::shiftClickRight()
{
    std::shared_ptr<Item> i = std::make_shared<Item>(*mousedItem());
    if (i) {
        float intermediate = i->count();
        intermediate /= 2.f;
        intermediate += 0.9f; //aggressive round up to take the bigger half of odd numbers
        size_t diff = intermediate;
        mousedCell()->take(diff);
        if (mousedItem()->count() == 0) {
            mousedCell()->clearItem();
        }
        i->setCount(diff);

        if (moused.x >= (int)inventory.rowCount) {
            // move half of stack to inventory
            inventory.addItem(i);
            if (i) {
                mousedCell()->add(i->count());
            }
            readInventory();
            writeExtension();
        }
        else if (moused.x >= 0) {
            // move half of stack to machine
            size_t remainder = machine->addReagant(i);
            if (remainder > 0) {
                mousedCell()->add(remainder);
            }
            writeInventory();
            readExtension();
        }
    }
}
