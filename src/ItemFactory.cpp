#include "ItemFactory.h"

std::unique_ptr<Item> ItemFactory::createItem(
    const std::string& type,
    int id,
    const std::string& name,
    const Stats& stats,
    const std::string& element,
    int rarity,
    int min_level,
    int max_level)
{
    if (type == "weapon")
        return std::make_unique<Weapon>(id, name, stats, element, rarity, min_level, max_level);
    else if (type == "armor")
        return std::make_unique<Armor>(id, name, stats, element, rarity, min_level, max_level);
    else if (type == "accessory")
        return std::make_unique<Accessory>(id, name, stats, element, rarity, min_level, max_level);

    throw std::runtime_error("Unknown item type: " + type);
}
