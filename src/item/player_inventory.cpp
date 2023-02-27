#include <item/player_inventory.hpp>

#include <util/primordial.hpp>

Player_Inventory::Player_Inventory()
{
    resize(rowCount, rowWidth);
    setEquipped(0);
}

void Player_Inventory::resize(const size_t rows, const size_t cols)
{
    rowCount = rows;
    rowWidth = cols;

    items.resize(rowCount);

    for (auto& row : items) {
        for (size_t i = row.size(); i < rowWidth; i++) {
            row.push_back(nullptr);
        }
    }
}

void Player_Inventory::removeItem(std::string name, size_t count)
{
    for (size_t x = 0; x < rowCount; x++) {
        for (size_t y = 0; y < rowWidth; y++) {
            if (items[x][y] && equalStrings(name, items[x][y]->getName())) {
                size_t i_count = items[x][y]->count();
                if (count <= i_count) {
                    items[x][y]->take(count);
                    return;
                }
                else {
                    count -= i_count;
                    items[x][y]->take(i_count);
                }

                if (items[x][y]->count() == 0) {
                    items[x][y].reset();
                }
            }
        }
    }
}

size_t Player_Inventory::countItem(std::string name)
{
    size_t count = 0;
    for (size_t x = 0; x < rowCount; x++) {
        for (size_t y = 0; y < rowWidth; y++) {
            if (items[x][y] && equalStrings(name, items[x][y]->getName())) {
                count += items[x][y]->count();
            }
        }
    }
    return count;
}

void Player_Inventory::addItem(std::shared_ptr<Item>& item)
{
    if (item) {
        size_t count = item->count();
        sf::Vector2i first_empty_index(-1, -1);
        item->can_pickup = true;
        for (size_t r = 0; r < rowCount; r++) {
            for (size_t c = 0; c < rowWidth; c++) {
                if (items[r][c] && item->getUID() == items[r][c]->getUID()) {
                    int stack = count + items[r][c]->count();
                    if (stack <= item->stackSize()) {
                        items[r][c]->add(count);
                        item = nullptr;
                        return;
                    }
                    else {
                        size_t diff = item->stackSize() - items[r][c]->count();
                        item->take(diff);
                        items[r][c]->add(diff);
                    }
                }
                else if (!items[r][c] && first_empty_index == sf::Vector2i(-1, -1)) {
                    first_empty_index = sf::Vector2i(r, c);
                }
            }
        }
        if (first_empty_index != sf::Vector2i(-1, -1) && item) {
            items[first_empty_index.x][first_empty_index.y] = item;
            item = nullptr;
        }
    }
}

size_t Player_Inventory::takeItem(size_t x, size_t y, size_t count)
{
    size_t remainder = 0;

    Item* i = items[x][y].get();

    if (i) {
        remainder = i->take(count);

        if (i->count() == 0) {
            items[x][y] = nullptr;
        }
    }

    return remainder;
}

void Player_Inventory::clearItem(size_t x, size_t y)
{
    items[x][y] = nullptr;
}

void Player_Inventory::placeItem(size_t x, size_t y, std::shared_ptr<Item> item)
{
    items[x][y].reset();
    items[x][y] = item;
    return;
    if (item) {
        items[x][y] = std::make_shared<Item>(*item);
    }
}

void Player_Inventory::update()
{
    for (size_t r = 0; r < rowCount; r++) {
        for (size_t c = 0; c < rowWidth; c++) {
            if (items[r][c]) {
                if (items[r][c]->count() == 0) {
                    items[r][c] = nullptr;
                }
            }
        }
    }
}

std::shared_ptr<Item> Player_Inventory::equippedItem()
{
    return items[equippedRow][equippedIndex];
}

void Player_Inventory::takeEquipped(int count)
{
    if (items[equippedRow][equippedIndex]) {
        if (count < 0) {
            count = equippedItem()->count();
        }
        takeItem(equippedRow, equippedIndex, count);
    }
}

std::shared_ptr<Item> Player_Inventory::findItem(std::string name)
{
    std::shared_ptr<Item> item = nullptr;
    for (auto& r : items) {
        for (auto& i : r) {
            if (i && equalStrings(name, i->getName())) {
                if (!item) {
                    item = i;
                    i = nullptr;
                }
                else {
                    size_t max = i->stackSize();
                    size_t c = i->count();
                    if (c + item->count() < max) {
                        item->add(c);
                        i = nullptr;
                    }
                    else {
                        size_t diff = max - item->count();
                        i->take(diff);
                        item->add(diff);
                        return item;
                    }
                }
            }
        }
    }
    return item;
}

void Player_Inventory::setEquipped(size_t index)
{
    if (index >= rowWidth) {
        index = 0;
    }

    equippedIndex = index;
}
