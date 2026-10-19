#pragma once
#include "Stats.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include <memory>
#include <string>

class ItemFactory {
public:
    static std::unique_ptr<Item> createItem(
        const std::string& type,
        int id,
        const std::string& name,
        const Stats& stats,
        const std::string& element,
        int rarity,
        int min_level,
        int max_level);
};
