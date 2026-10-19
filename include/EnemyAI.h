#pragma once

#include "EnemyPersonality.h"

class Player;
class Enemy;
struct CombatState;


enum class EnemyAction {
    Attack,
    Defend
};

class EnemyAI {
public:
    EnemyAI() = default;

    EnemyAction chooseAction(
        const Player& p,
        const Enemy& e,
        const CombatState& playerState,
        const CombatState& enemyState
    ) const;

private:
    EnemyAction chooseEnraged() const;
    EnemyAction chooseSubdued() const;
    EnemyAction chooseTactical(const CombatState& playerState) const;
    EnemyAction chooseCautious(const Enemy& e) const;
};
