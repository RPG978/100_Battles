#pragma once
#include "Player.h"
#include "Enemy.h"
#include "CombatState.h"
#include "EnemyAI.h"
#include <string>

class CombatEngine {
public:
    // Initialize combat state
    void start(Player& p, Enemy& e);
    void reset();

    // Player actions
    void playerAttack(Player& p, Enemy& e);
    void playerDefend(Player& p, Enemy& e);
    void useItem(Player& p, const std::string& item);
    void tryRun(Player& p, Enemy& e);

    // Query
    bool isCombatOver() const { return combatOver; }

    bool playerDied() const { return playerDead; }
    bool enemyDied() const { return enemyDead; }
    void handlePlayerDeath(Player& player);

    int getLastLostGold() const { return lastLostGold; }
    int getLastLostXP() const { return lastLostXP; }

    const CombatState& getPlayerState() const { return playerState; }
    const CombatState& getEnemyState() const { return enemyState; }

    int testEnemyAttack(Player& p, Enemy& e) {
        int before = p.getCurrentHealth();
        enemyAttack(p, e);
        return before - p.getCurrentHealth();
    }

    int testEnemyTurn(Player& p, Enemy& e) {
        int before = p.getCurrentHealth();
        enemyTurn(p, e);
        return before - p.getCurrentHealth();
    }

    int testCalculateEnemyDamage(Player& p, Enemy& e, bool playerDefending, int defendsInRow) {
        playerDefending = playerDefending;
        playerState.defendsInRow = defendsInRow;
        int before = p.getCurrentHealth();
        enemyAttack(p, e);
        return before - p.getCurrentHealth();
    }

#ifdef UNIT_TEST
    CombatState& getMutablePlayerState() { return playerState; }
    CombatState& getMutableEnemyState() { return enemyState; }

    int testDefendDamage(const Stats& attackerStats,
        int defenderDefense,
        bool defenderDefending,
        int defendsInRow,
        bool attackerAgro,
        bool defenderAgroFlag)
    {
        return calculateDamage(attackerStats,
            defenderDefense,
            defenderDefending,
            defendsInRow,
            attackerAgro,
            defenderAgroFlag);
    }

    int testAttackDamage(const Stats& attackerStats,
        int defenderDefense,
        bool defenderDefending,
        int defendsInRow,
        bool attackerAgro,
        bool defenderAgroFlag)
    {
        return calculateDamage(attackerStats,
            defenderDefense,
            defenderDefending,
            defendsInRow,
            attackerAgro,
            defenderAgroFlag);
    }

    int testEnemyAttack(const Stats& attackerStats,
        int defenderDefense,
        bool defenderDefending,
        int defendsInRow,
        bool attackerAgro,
        bool defenderAgroFlag) {
        return calculateDamage(attackerStats,
            defenderDefense,
            defenderDefending,
            defendsInRow,
            attackerAgro,
            defenderAgroFlag);
    }
#endif

private:
    // Internal enemy turn
    void enemyTurn(Player& p, Enemy& e);

    // Enemy actions
    void enemyAttack(Player& p, Enemy& e);
    void enemyDefend(Player& p, Enemy& e);

    // Momentum / streak logic
    void updateCombatState(CombatState& state, bool didAttack, bool didDefend);

    // Combat state
    bool playerDefending = false;
    bool enemyDefending = false;
    bool combatOver = false;
    bool playerDead = false;
    bool enemyDead = false;

    int calculateDamage(const Stats& attackerStats,
        int defenderDefense,
        bool defenderDefending,
        int defendsInRow,
        bool attackerAgro,
        bool defenderAgroFlag);

    void awardRewards(Player& player, const Enemy& enemy);
    int lastLostGold = 0;
    int lastLostXP = 0;

    CombatState playerState;
    CombatState enemyState;

    EnemyAI ai;
};
