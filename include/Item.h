#pragma once
#include "Stats.h"
#include <string>

class Item {
public:
    virtual ~Item() = default;

    int id;
    std::string name;
    Stats stats;
    std::string element;
    int rarity;
    int min_level;
    int max_level;

};
