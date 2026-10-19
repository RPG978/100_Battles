#pragma once
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "Stats.h"
#include <string>

class Player {
public:
    Player();
    Player(const std::string& name, const Stats& stats);
    Player(int id,
        const std::string& name,
        Stats stats,
        const std::string& attack_element,
        const std::string& defense_element,
        const std::string& counter_element);

    // Equipment
    void equipWeapon(Weapon* w);
    void equipArmor(Armor* a);
    void equipAccessory(Accessory* acc);

    // Derived stats
    Stats getTotalStats() const;
    int getTotalAttack() const { return getTotalStats().attack; }
    int getTotalDefense() const { return getTotalStats().defense; }

    // HP management
    int getCurrentHealth() const { return current_health; }
    int getMaxHealth() const { return max_health; }
    void setCurrentHealth(int h) { current_health = h; }
    void recalculateHealth();   // updates max_health + current_health

    // Progression
    int getGold() const { return gold; }
    void setGold(int amount) { gold = amount; }
    void addGold(int amount) { gold += amount; }
    void spendGold(int amount) { gold = std::max(0, gold - amount); }
    
    int getXP() const { return xp; }
    int getLevel() const { return level; }
    void addXP(int amount);
    void levelUp();
    void allocateStatPoint(const std::string& stat);
    int getUnspentPoints() const { return unspent_points; }
    void deductXP(int amount);

    // Runtime
    bool defending;

    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const Stats& getStats() const { return stats; }
    const std::string& getAttackElement() const { return attack_element; }
    const std::string& getDefenseElement() const { return defense_element; }
    const std::string& getCounterElement() const { return counter_element; }
    Weapon* getWeapon() const { return equippedWeapon; }
    Armor* getArmor() const { return equippedArmor; }
    Accessory* getAccessory() const { return equippedAccessory; }

private:
    int id;
    std::string name;
    Stats stats;

    std::string attack_element;
    std::string defense_element;
    std::string counter_element;

    int current_health;
    int max_health;

    Weapon* equippedWeapon;
    Armor* equippedArmor;
    Accessory* equippedAccessory;

    int gold;
    int xp;
    int level;
    int unspent_points = 0;
};
