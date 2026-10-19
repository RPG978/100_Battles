#include "Weapon.h"

Weapon::Weapon(int id,
    const std::string& name,
    const Stats& stats,
    std::string element,
    int rarity,
    int min_level,
    int max_level)
    : id(id),
    name(name),
    stats(stats),
    element(element),
    rarity(rarity),
    min_level(min_level),
    max_level(max_level)
{
}
