#pragma once
#include "Stats.h"
#include "Item.h"
#include <string>

class Armor : public Item {
public:
    int id;
    std::string name;
    Stats stats;

    std::string element;
    int rarity;
    int min_level;
    int max_level;

    Armor(int id,
        const std::string& name,
        const Stats& stats,
        std::string element,
        int rarity,
        int min_level,
        int max_level);

    //Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    Stats getStats() const { return stats; }
    const std::string& getElement() const { return element; }
    int getRarity() const { return rarity; }
    int getMinLevel() const { return min_level; }
    int getMaxLevel() const { return max_level; }
};
