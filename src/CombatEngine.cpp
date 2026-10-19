#include "CombatEngine.h"
#include "Random.h"
#include <algorithm>
#include <iostream>

void CombatEngine::start(Player& p, Enemy& e)
{
    playerDead = false;
    enemyDead = false;
    combatOver = false;
    playerDefending = false;
    enemyDefending = false;

    playerState = CombatState{};
    enemyState = CombatState{};

    // Force recalculation if max hp is 0
    if (p.getMaxHealth() <= 0)
        p.recalculateHealth();

    // Ensure enemy HP is initialized
    if (e.max_health <= 0)
    {
        e.max_health = e.stats.health;
        e.current_health = e.stats.health;
    }

    playerState.health = p.getCurrentHealth();
    playerState.maxHealth = p.getMaxHealth();

    enemyState.health = e.getCurrentHealth();
    enemyState.maxHealth = e.getMaxHealth();
}

void CombatEngine::reset()
{
    combatOver = false;

    playerDefending = false;
    enemyDefending = false;

    playerState = CombatState{};
    enemyState = CombatState{};
}

int CombatEngine::calculateDamage(const Stats& attackerStats,
    int defenderDefense,
    bool defenderDefending,
    int defendsInRow,
    bool attackerAgro,
    bool defenderAgroFlag) {
    float defenseMultiplier = defenderAgroFlag ? 0.5f : 1.0f;

    float dmgMultiplier = std::max(
        0.0f,
        1.0f - (defenderDefense * defenseMultiplier) * 0.01f
    );

    if (defenderDefending) {
        if (defendsInRow == 1) {
            dmgMultiplier *= 0.5f;
        }
        else if (defendsInRow >= 2) {
            dmgMultiplier = 0.0f; // full block
        }
    }

    int dmgTotal = roll(attackerStats.attack * 0.9, attackerStats.attack * 1.1) * dmgMultiplier;

    if (attackerAgro) {
        dmgTotal *= 2;
    }

    return dmgTotal;
}

void CombatEngine::playerAttack(Player& p, Enemy& e) {
    if (combatOver) return;

    updateCombatState(playerState, true, false);

    if (playerState.isNextAttackTranquil) {
        std::cout << p.getName() << " is tranquil and cannot act!\n";
        playerState.isNextAttackTranquil = false;
        enemyTurn(p, e);
        return;
    }

    int dmgTotal = calculateDamage(
        p.getTotalStats(),
        e.getStats().defense,
        enemyDefending,
        enemyState.defendsInRow,
        playerState.isAgro,
        enemyState.isNextAttackAgro
    );

    if (playerState.isAgro) {
        std::cout << p.getName() << " unleashes an Agro strike! (x2 damage)\n";
        playerState.isAgro = false;
    }

    std::cout << p.getName() << " attacks " << e.getName()
        << " for " << dmgTotal << "\n";

    e.current_health -= dmgTotal;
    if (e.current_health < 0) e.current_health = 0;

    std::cout << e.getName() << "'s HP: " << e.current_health << "\n";

    enemyState.health = e.current_health;
    playerState.health = p.getCurrentHealth();

    if (e.current_health <= 0) {
        std::cout << e.getName() << " has been defeated!\n";
        awardRewards(p, e);
        enemyDead = true;
        combatOver = true;
        return;
    }

    enemyState.isNextAttackAgro = false;

    enemyTurn(p, e);
}

void CombatEngine::playerDefend(Player& p, Enemy& e) {
    if (combatOver) return;

    // Tranquil: player loses their turn
    if (playerState.isNextAttackTranquil) {
        std::cout << p.getName() << " is tranquil and cannot act!\n";
        
        playerState.isNextAttackTranquil = false;
        playerState.isTranquil = false;
        playerState.defendsInRow = 0;

        enemyTurn(p, e);
        return;
    }

    // Mark that the player is defending this turn
    playerDefending = true;
    std::cout << p.getName() << " braces for impact!\n";

    // Update streaks (attack = false, defend = true)
    updateCombatState(playerState, false, true);

    // Reset one-turn Agro flag (defending breaks Agro momentum)
    playerState.isNextAttackAgro = false;

    // Enemy takes their turn
    enemyTurn(p, e);
}

void CombatEngine::enemyTurn(Player& p, Enemy& e) {

    EnemyAction action = ai.chooseAction(p, e, playerState, enemyState);

    if (action == EnemyAction::Attack)
        enemyAttack(p, e);
    else
        enemyDefend(p, e);
}

void CombatEngine::enemyAttack(Player& p, Enemy& e) {
    if (combatOver) return;

    updateCombatState(enemyState, true, false);

    if (enemyState.isNextAttackTranquil) {
        std::cout << e.getName() << " is tranquil and cannot act!\n";
        enemyState.isNextAttackTranquil = false;
        enemyState.isTranquil = false;
        return;
    }

    int dmgTotal = calculateDamage(
        e.getStats(),
        p.getTotalDefense(),
        playerDefending,
        playerState.defendsInRow,
        enemyState.isAgro,
        playerState.isNextAttackAgro
    );

    if (enemyState.isAgro) {
        std::cout << e.getName() << " unleashes an Agro strike! (x2 damage)\n";
        enemyState.isAgro = false;
    }

    std::cout << e.getName() << " attacks " << p.getName()
        << " for " << dmgTotal << "\n";

    p.setCurrentHealth(p.getCurrentHealth() - dmgTotal);
    if (p.getCurrentHealth() < 0) p.setCurrentHealth(0);

    std::cout << p.getName() << "'s HP: " << p.getCurrentHealth() << "\n";

    playerDefending = false;

    playerState.health = p.getCurrentHealth();
    enemyState.health = e.current_health;

    if (p.getCurrentHealth() <= 0) {
        std::cout << p.getName() << " has been defeated!\n";
        playerDead = true;
        combatOver = true;

        handlePlayerDeath(p);

        return;
    }

    playerState.isNextAttackAgro = false;
}

void CombatEngine::enemyDefend(Player& p, Enemy& e) {
    if (combatOver) return;

    std::cout << e.getName() << " braces for impact!\n>>";

    // Update streaks (enemy defended)
    updateCombatState(enemyState, false, true);

    // Defending breaks Agro momentum
    enemyState.isNextAttackAgro = false;

    // Mark enemy as defending for the next incoming attack
    enemyDefending = true;

    // If enemy was tranquil, consume it (mirrors playerDefend)
    if (enemyState.isNextAttackTranquil) {
        enemyState.isNextAttackTranquil = false;
        enemyState.isTranquil = false;
        enemyState.defendsInRow = 0;
    }

    // Enemy does NOT attack this turn — player gets next move
}

void CombatEngine::tryRun(Player& p, Enemy& e) {
    if (combatOver) return;

    if (roll(1, 100) <= 40) {
        std::cout << p.getName() << " successfully escaped!\n";
        combatOver = true;
    }
    else {
        std::cout << p.getName() << " failed to escape!\n";
        enemyTurn(p, e);
    }

    // sync snapshot
    playerState.health = p.getCurrentHealth();
    enemyState.health = e.current_health;
}

void CombatEngine::useItem(Player& p, const std::string& item) {
    std::cout << p.getName() << " tries to use " << item << "...\n";
    std::cout << "But items aren't implemented yet.\n";
}

void CombatEngine::updateCombatState(CombatState& state, bool didAttack, bool didDefend) {

    if (didAttack) {
        state.attacksInRow++;
        state.defendsInRow = 0;

        if (state.attacksInRow == 3) {
            state.isAgro = true;
            state.isNextAttackAgro = true;
            state.attacksInRow = 0;
        }
    }
    else if (didDefend) {
        state.defendsInRow++;
        state.attacksInRow = 0;

        if (state.defendsInRow == 3) {
            state.isTranquil = true;
            state.isNextAttackTranquil = true;

            std::cout << "You enter a Tranquil state! Your next turn will be skipped.\n";
        }
    }
    else {
        state.attacksInRow = 0;
        state.defendsInRow = 0;
    }
}

void CombatEngine::awardRewards(Player& player, const Enemy& enemy) {
    player.addGold(enemy.rewardGold);
    player.addXP(enemy.rewardXP);

    if (enemy.rewardLoot) {
        player.equipWeapon(enemy.rewardLoot); // minimalist for now
    }
}

void CombatEngine::handlePlayerDeath(Player& player)
{
    int lostGold = player.getGold() / 2;
    player.spendGold(lostGold);

    int lostXP = player.getXP() / 4;
    player.deductXP(lostXP);

    player.setCurrentHealth(player.getMaxHealth() / 2);

    // Store penalty values locally
    lastLostGold = lostGold;
    lastLostXP = lostXP;
}

