#include <catch2/catch_test_macros.hpp>
#include "CombatEngine.h"
#include "Player.h"
#include "Enemy.h"
#include "Random.h"

// Tests player attack flow
TEST_CASE("Attack reduces enemy HP") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Slime", Stats{ 50, 15, 5 }, "None", "None", "None", 1, 1, 1);
    CombatEngine engine;

    engine.start(p, e);
    engine.playerAttack(p, e);

    REQUIRE(e.getCurrentHealth() < e.getMaxHealth());
}

// Tests damage mechanic
TEST_CASE("Defend streak reduces damage correctly") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Spiked Lizard", Stats{ 50, 15, 5 },
        "None", "None", "None", 1, 1, 1);

    CombatEngine engine;
    engine.start(p, e);

    const Stats& enemyStats = e.getStats();
    int defenderDefense = p.getTotalDefense();

    int dmgBaseline = engine.testDefendDamage(enemyStats,
        defenderDefense,
        false,   // not defending
        0,       // no streak
        false,   // enemy not agro
        false);  // player not agro

    int dmgDefend1 = engine.testDefendDamage(enemyStats,
        defenderDefense,
        true,    // defending
        1,       // first defend
        false,
        false);

    int dmgDefend2 = engine.testDefendDamage(enemyStats,
        defenderDefense,
        true,    // defending
        2,       // second defend
        false,
        false);

    REQUIRE(dmgDefend1 < dmgBaseline);
    REQUIRE(dmgDefend2 == 0);
}

// Tests combat flow and defend flag behavior
TEST_CASE("Attack vs Defend paths") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Slime", Stats{ 50, 15, 5 }, "None", "None", "None", 1, 1, 1);
    CombatEngine engine;
    engine.start(p, e);

    int enemyHPBefore = e.getCurrentHealth();
    engine.playerAttack(p, e);
    REQUIRE(e.getCurrentHealth() < enemyHPBefore);

    int heroHPBefore = p.getCurrentHealth();
    engine.playerDefend(p, e);
    engine.playerAttack(p, e); // enemy turn triggers
    REQUIRE(p.getCurrentHealth() <= heroHPBefore);
}

// Tests combat loop stability
TEST_CASE("Repeated attacks reduce enemy HP") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Slime", Stats{ 200, 15, 5 }, "None", "None", "None", 1, 1, 1);
    CombatEngine engine;
    engine.start(p, e);

    int startingHP = e.getCurrentHealth();
    for (int i = 0; i < 100; ++i) {
        engine.playerAttack(p, e);
        if (engine.isCombatOver()) break;
    }

    REQUIRE(e.getCurrentHealth() <= startingHP);
}

// Tests deterministic math
TEST_CASE("Agro streak doubles damage") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Wolf", Stats{ 50, 15, 5 },
        "None", "None", "None", 1, 1, 1);

    CombatEngine engine;
    engine.start(p, e);

    const Stats& playerStats = p.getTotalStats();
    int defenderDefense = e.getStats().defense;

    setForcedRoll(20);   // deterministic roll

    int dmgBaseline = engine.testAttackDamage(
        playerStats,
        defenderDefense,
        false,
        0,
        false,
        false
    );

    engine.getMutablePlayerState().attacksInRow = 3;
    engine.getMutablePlayerState().isAgro = true;

    int dmgAgro = engine.testAttackDamage(
        playerStats,
        defenderDefense,
        false,
        0,
        true,
        false
    );

    clearForcedRoll();

    REQUIRE(dmgAgro == dmgBaseline * 2);
}

// Ensure enemy can agro
TEST_CASE("Enemy Agro streak doubles damage") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Orc", Stats{ 50, 15, 5 }, "None", "None", "None", 1, 1, 1);
    CombatEngine engine;
    engine.start(p, e);

    const Stats& enemyStats = e.getStats();
    int defenderDefense = p.getTotalDefense();

    setForcedRoll(15); // deterministic roll

    // Baseline damage (no Agro)
    int dmgBaseline = engine.testDefendDamage(
        enemyStats,
        defenderDefense,
        false,   // player not defending
        0,       // no defend streak
        false,   // enemy not agro
        false    // player agro flag irrelevant
    );

    // Simulate Agro streak
    engine.getMutableEnemyState().attacksInRow = 3;
    engine.getMutableEnemyState().isAgro = true;

    int dmgAgro = engine.testDefendDamage(
        enemyStats,
        defenderDefense,
        false,
        0,
        true,    // enemy agro active
        false
    );

    clearForcedRoll();

    REQUIRE(dmgAgro == dmgBaseline * 2);
}

// Tests meaningful flow behavior
TEST_CASE("Tranquil streak skips next turn") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Wolf", Stats{ 50, 15, 5 },
        "None", "None", "None", 1, 1, 1);

    CombatEngine engine;
    engine.start(p, e);

    // Simulate 3 consecutive defends -> Tranquil triggers
    engine.getMutablePlayerState().defendsInRow = 3;
    engine.getMutablePlayerState().isNextAttackTranquil = true;

    // Player tries to attack — should be skipped
    int hpBefore = e.getCurrentHealth();
    engine.playerAttack(p, e);  // tranquil should block this action
    int hpAfter = e.getCurrentHealth();

    REQUIRE(hpAfter == hpBefore);  // no damage dealt
}

// Ensure enemy becomes tranquil
TEST_CASE("Enemy Tranquil streak skips next turn") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Orc", Stats{ 50, 15, 5 }, "None", "None", "None", 1, 1, 1);
    CombatEngine engine;
    engine.start(p, e);

    // Simulate tranquil streak
    engine.getMutableEnemyState().defendsInRow = 3;
    engine.getMutableEnemyState().isNextAttackTranquil = true;

    int hpBefore = p.getCurrentHealth();
    engine.testEnemyAttack(p, e);  // tranquil should block this action
    int hpAfter = p.getCurrentHealth();

    REQUIRE(hpAfter == hpBefore);  // no damage dealt
}

// Tests combat calculation mechanic on enemy
TEST_CASE("Enemy defend reduces incoming damage") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy e(1, "Goblin", Stats{ 50, 15, 5 }, "None", "None", "None", 1, 1, 1);
    CombatEngine engine;
    engine.start(p, e);

    const Stats& playerStats = p.getTotalStats();
    int defenderDefense = e.getStats().defense;

    setForcedRoll(20); // deterministic roll

    int dmgBaseline = engine.testAttackDamage(
        playerStats,
        defenderDefense,
        false,   // enemy not defending
        0,
        false,
        false
    );

    int dmgDefend = engine.testAttackDamage(
        playerStats,
        defenderDefense,
        true,    // enemy defending
        1,       // first defend streak
        false,
        false
    );

    clearForcedRoll();

    REQUIRE(dmgDefend < dmgBaseline);
}

// Tests stat effects mechanic
TEST_CASE("Defense multiplier reduces damage with higher defense") {
    Player p("Hero", Stats{ 100, 20, 10 });
    Enemy eLowDef(1, "Rat", Stats{ 50, 15, 2 }, "None", "None", "None", 1, 1, 1);
    Enemy eHighDef(2, "Knight", Stats{ 50, 15, 20 }, "None", "None", "None", 1, 1, 1);
    CombatEngine engine;
    engine.start(p, eLowDef);

    const Stats& playerStats = p.getTotalStats();

    setForcedRoll(20);

    int dmgLowDef = engine.testAttackDamage(
        playerStats,
        eLowDef.getStats().defense,
        false,
        0,
        false,
        false
    );

    int dmgHighDef = engine.testAttackDamage(
        playerStats,
        eHighDef.getStats().defense,
        false,
        0,
        false,
        false
    );

    clearForcedRoll();

    REQUIRE(dmgHighDef < dmgLowDef);
}

// Tests combat algorithm stability
TEST_CASE("Damage stays within expected range") {
    Player p("Hero", Stats{100, 20, 10});
    Enemy e(1, "Slime", Stats{50, 15, 5}, "None","None","None",1,1,1);
    CombatEngine engine;
    engine.start(p, e);

    const Stats& playerStats = p.getTotalStats();
    int defenderDefense = e.getStats().defense;

    // Force multiple rolls to check range
    for (int forced = 18; forced <= 22; ++forced) {
        setForcedRoll(forced);

        int dmg = engine.testAttackDamage(
            playerStats,
            defenderDefense,
            false,
            0,
            false,
            false
        );

        // Expected range: attack * 0.9 to attack * 1.1, after defense multiplier
        int minExpected = static_cast<int>(playerStats.attack * 0.9 * (1.0 - defenderDefense * 0.01));
        int maxExpected = static_cast<int>(playerStats.attack * 1.1 * (1.0 - defenderDefense * 0.01));

        REQUIRE(dmg >= minExpected);
        REQUIRE(dmg <= maxExpected);
    }

    clearForcedRoll();
}
