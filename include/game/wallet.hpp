#pragma once

#include <functional>

class Wallet {
public:
    Wallet();

    int getBalance();
    void setBalance(int balance);

    void add(int diff);

    void take(int diff);

    std::function<void(int)> update_ui;

private:
    int balance = 0;
};
