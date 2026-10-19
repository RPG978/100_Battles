#pragma once

struct CombatState {
    int health = 0;
    int maxHealth = 0;

    int attacksInRow = 0;
    int defendsInRow = 0;

    bool isAgro = false;
    bool isNextAttackAgro = false;

    bool isTranquil = false;
    bool isNextAttackTranquil = false;
};
