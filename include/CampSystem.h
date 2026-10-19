#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"

//--------------------------------
// The CampSystem is responsible for handling post-combat progression and recovery.
//--------------------------------
class CampSystem {
public:
    CampSystem(Player& player);

    // Healing
    void healPlayer();

    // Rewards
    void awardRewards(int gold, int xp, Weapon* loot = nullptr);

    // Leveling
    void levelUpPlayer();

    // Shop
    bool buyWeapon(const Weapon& weapon);

private:
    Player& player;
    int xpThreshold = 100; // Example threshold per level
};