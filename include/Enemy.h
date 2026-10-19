#pragma once
#include "Stats.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "EnemyPersonality.h"
#include <string>

class Enemy {
public:
	Enemy(int id,
		const std::string& name,
		Stats stats,
		const std::string& attack_element,
		const std::string& defense_element,
		const std::string& counter_element,
		int rarity,
		int min_level,
		int max_level);

	Enemy(int id,
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
		Weapon* rewardLoot);

	// Rewards
	int rewardGold;
	int rewardXP;
	Weapon* rewardLoot;

	int getRewardGold() const { return rewardGold; }
	int getRewardXP() const { return rewardXP; }
	Weapon* getRewardLoot() const { return rewardLoot; }

	Stats stats;

	int current_health;
	int max_health;

	int getCurrentHealth() const { return current_health; }
	int getMaxHealth() const { return max_health; }

	const Stats& getTotalStats() const { return stats; }

	//Getters
	int getId() const { return id; }
	const std::string& getName() const { return name; }
	const Stats& getStats() const { return stats; }
	const std::string& getAttackElement() const { return attack_element; }
	const std::string& getDefenseElement() const { return defense_element; }
	const std::string& getCounterElement() const { return counter_element; }
	int getRarity() const { return rarity; }
	int getMinLevel() const { return min_level; }
	int getMaxLevel() const { return max_level; }

	void printStats() const;

	EnemyPersonality getPersonality() const { return personality; }
	void setPersonality(EnemyPersonality p) { personality = p; }
private:
	int id;
	std::string name;
	std::string attack_element;
	std::string defense_element;
	std::string counter_element;
	int rarity;
	int min_level;
	int max_level;
	EnemyPersonality personality = EnemyPersonality::Enraged;
};