#include "Player.h"

Player::Player()
    : 
    id(0), 
    name(""), 
    stats{}, 
    attack_element("None"), 
    defense_element("None"),
    counter_element("None"), 
    current_health(0), 
    max_health(0), 
    defending(false),
    equippedWeapon(nullptr), 
    equippedArmor(nullptr), 
    equippedAccessory(nullptr),
    gold(0), 
    xp(0), 
    level(1)
{
}

Player::Player(const std::string& name, const Stats& stats)
    : 
    id(0), 
    name(name), 
    stats(stats), 
    attack_element("None"), 
    defense_element("None"),
    counter_element("None"), 
    current_health(stats.health), 
    max_health(stats.health),
    defending(false), 
    equippedWeapon(nullptr), 
    equippedArmor(nullptr), 
    equippedAccessory(nullptr),
    gold(0), 
    xp(0), 
    level(1)
{
}

Player::Player(int id,
    const std::string& name,
    Stats stats,
    const std::string& attack_element,
    const std::string& defense_element,
    const std::string& counter_element)
    :
    id(id),
    name(name),
    stats(stats),
    attack_element(attack_element.empty() ? "None" : attack_element),
    defense_element(defense_element.empty() ? "None" : defense_element),
    counter_element(counter_element.empty() ? "None" : counter_element),
    current_health(stats.health),
    max_health(stats.health),
    equippedWeapon(nullptr),
    equippedArmor(nullptr),
    equippedAccessory(nullptr),
    defending(false),
    gold(0),
    xp(0),
    level(1)
{
}

void Player::equipWeapon(Weapon* w) {
    equippedWeapon = w;
    if (w) attack_element = w->element;
}

void Player::equipArmor(Armor* a) {
    equippedArmor = a;
    if (a) defense_element = a->element;
}

void Player::equipAccessory(Accessory* acc) {
    equippedAccessory = acc;
    if (acc) counter_element = acc->element;
}

Stats Player::getTotalStats() const {
    Stats total = stats;

    if (equippedWeapon) {
        total.attack += equippedWeapon->stats.attack;
        total.defense += equippedWeapon->stats.defense;
        total.health += equippedWeapon->stats.health;
    }

    if (equippedArmor) {
        total.attack += equippedArmor->stats.attack;
        total.defense += equippedArmor->stats.defense;
        total.health += equippedArmor->stats.health;
    }

    if (equippedAccessory) {
        total.attack += equippedAccessory->stats.attack;
        total.defense += equippedAccessory->stats.defense;
        total.health += equippedAccessory->stats.health;
    }

    return total;
}

void Player::recalculateHealth() {
    int oldMax = max_health;
    max_health = getTotalStats().health;

    if (oldMax > 0) {
        float ratio = static_cast<float>(current_health) / oldMax;
        current_health = static_cast<int>(ratio * max_health);
    }
    else {
        current_health = max_health;
    }
}


void Player::addXP(int amount) {
    xp += amount;

    // Check for level-up
    while (xp >= level * 100) {
        xp -= level * 100;
        levelUp();
    }
}

void Player::levelUp() {
    level++;
    unspent_points += 3;

    // Optional: heal to full
    current_health = max_health;
}

void Player::allocateStatPoint(const std::string& stat) {
    if (unspent_points <= 0) return;

    if (stat == "HP") {
        stats.health += 10;
        max_health += 10;
        current_health += 10; // optional
    }
    else if (stat == "Attack") {
        stats.attack += 1;
    }
    else if (stat == "Defense") {
        stats.defense += 1;
    }

    unspent_points--;
}

void Player::deductXP(int amount) {
    xp -= amount;
    if (xp < 0) xp = 0; // clamp so it never goes negative
}
