#pragma once

#include <vector>

struct Reagant {
    std::string name;
    short unsigned int count = 1;

    Reagant() = default;
    Reagant(std::string n)
        : name { n }
    {}
};

struct Reaction {
    std::string name;
    std::vector<Reagant> reagants;
    std::string product;
    size_t length = 1;
    std::string tag;
    bool operator <  (const Reaction& r) { return tag <  r.tag; }
    bool operator >  (const Reaction& r) { return tag >  r.tag; }
    bool operator <= (const Reaction& r) { return tag <= r.tag; }
    bool operator >= (const Reaction& r) { return tag >= r.tag; }
};
