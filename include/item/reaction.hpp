#pragma once

#include <vector>

/// REAGANT ///
///
/// \brief information about an item used to perform reactions
///
struct Reagant {
    std::string name;
    short unsigned int count = 1; /**< amount consumed by the reaction */

    Reagant() = default;
    Reagant(std::string n)
        : name { n }
    {}
};

struct Reaction {
    std::string name;
    std::vector<Reagant> reagants; /**< items used in the reaction */
    std::string product; /**< item produced in the reaction */
    size_t length = 1; /**< reaction length in ticks */
    std::string tag; /**< for organizing in the reaction interface */

// comparators by tag, again for the reaction interface
    bool operator <  (const Reaction& r) { return tag <  r.tag; }
    bool operator >  (const Reaction& r) { return tag >  r.tag; }
    bool operator <= (const Reaction& r) { return tag <= r.tag; }
    bool operator >= (const Reaction& r) { return tag >= r.tag; }
};
