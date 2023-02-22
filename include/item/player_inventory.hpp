#pragma once

#include <memory>
#include <vector>

#include "item.hpp"

class Player_Inventory {
public:
    Player_Inventory();

    size_t rowCount = 4;
    size_t rowWidth = 8;

    std::shared_ptr<Item> operator()(const size_t row, const size_t col) { return items[row][col]; }

    void resize(const size_t rows, const size_t cols);

    void addItem(std::shared_ptr<Item>& item);
    size_t takeItem(size_t x, size_t y, size_t count);
    void clearItem(size_t x, size_t y);
    void placeItem(size_t x, size_t y, std::shared_ptr<Item> item);

    std::shared_ptr<Item> equippedItem();

    void setEquipped(size_t index);
    void takeEquipped(int count = -1);

    void update();

    void removeItem(std::string name, size_t count);

    size_t countItem(std::string name);

    std::shared_ptr<Item> findItem(std::string name);

private:
    std::vector<std::vector<std::shared_ptr<Item>>> items;

    const static unsigned int equippedRow = 0;
    size_t equippedIndex;
};

// add vector<v2i> changeIndices for UI reading
