#include <game/wallet.hpp>

Wallet::Wallet()
{
    update_ui = [](int){};
}

int Wallet::getBalance()
{
    return balance;
}

void Wallet::setBalance(int balance)
{
    this->balance = balance;

    update_ui(balance);
}

void Wallet::add(int diff)
{
    balance += diff;

    update_ui(balance);
}

void Wallet::take(int diff)
{
    if (diff > balance) {
        // log this
    }

    balance -= diff;
    if (balance < 0) {
        balance = 0;
    }

    update_ui(balance);
}
