#include <item/player_inventory.hpp>

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

    changed = true;
}

void Player_Inventory::addItem(Item* item, size_t count)
{
    if (item) {
        item->can_pickup = true;
        for (auto& i : items) {
            for (auto& j : i) {
                if (j && item->getUID() == j->getUID()) {
                    j->add(count);
                    changed = true;
                    return;
                }
                else if (j == nullptr) {
                    j = std::make_shared<Item>(*item);
                    j->setCount(count);
                    changed = true;
                    return;
                }
            }
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

    changed = true;

    return remainder;
}

void Player_Inventory::clearItem(size_t x, size_t y)
{
    items[x][y] = nullptr;
}

void Player_Inventory::placeItem(size_t x, size_t y, Item* item)
{
    items[x][y] = std::make_shared<Item>(*item);
}

void Player_Inventory::update()
{
    for (size_t r = 0; r < rowCount; r++) {
        for (size_t c = 0; c < rowWidth; c++) {
            if (items[r][c]) {
                if (items[r][c]->count() == 0) {
                    items[r][c] = nullptr;
                    changed = true;
                }
            }
        }
    }
}

Item* Player_Inventory::equippedItem()
{
    return items[equippedRow][equippedIndex].get();
}

void Player_Inventory::setEquipped(size_t index)
{
    if (index >= rowWidth) {
        index = 0;
    }

    equippedIndex = index;
}
