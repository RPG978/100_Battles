#include "Enemy.h"
#include "Random.h"

Enemy::Enemy(int id,
    const std::string& name,
    Stats stats,
    const std::string& attack_element,
    const std::string& defense_element,
    const std::string& counter_element,
    int rarity,
    int min_level,
    int max_level)
    :
    id(id),
    name(name),
    stats(stats),
    attack_element(attack_element.empty() ? "None" : attack_element),
    defense_element(defense_element.empty() ? "None" : defense_element),
    counter_element(counter_element.empty() ? "None" : counter_element),
    rarity(rarity),
    min_level(min_level),
    max_level(max_level),
    rewardGold(rarity* roll(5, 15)),
    rewardXP(rarity* roll(10, 25)),
    rewardLoot(nullptr)
{
    // Apply runtime variance
    stats.health += roll(-20, 20);
    stats.attack += roll(-2, 2);
    stats.defense += roll(-2, 2);

    // Clamp
    if (stats.health < 1) stats.health = 1;
    if (stats.attack < 0) stats.attack = 0;
    if (stats.defense < 0) stats.defense = 0;

    // initialize HP
    max_health = stats.health;
    current_health = stats.health;
}


Enemy::Enemy(int id,
    const std::string& name,
    Stats stats,
    const std::string& attack_element,
    const std::string& defense_element,
    const std::string& counter_element,
    int rarity,
    int min_level,
    int max_level,
    int rewardGold,
    int rewardXP,
    Weapon* rewardLoot)
    :
    id(id),
    name(name),
    stats(stats),
    attack_element(attack_element.empty() ? "None" : attack_element),
    defense_element(defense_element.empty() ? "None" : defense_element),
    counter_element(counter_element.empty() ? "None" : counter_element),
    rarity(rarity),
    min_level(min_level),
    max_level(max_level),
    rewardGold(rewardGold),
    rewardXP(rewardXP), 
    rewardLoot(rewardLoot)
{
    stats.health += roll(-20, 20);
    stats.attack += roll(-2, 2);
    stats.defense += roll(-2, 2);

    if (stats.health < 1) stats.health = 1;
    if (stats.attack < 0) stats.attack = 0;
    if (stats.defense < 0) stats.defense = 0;

    max_health = stats.health;
    current_health = stats.health;
}

