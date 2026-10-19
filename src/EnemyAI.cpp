#pragma once
#include "Player.h"
#include "Enemy.h"
#include "CombatState.h"
#include "Random.h"
#include "EnemyAI.h"

EnemyAction EnemyAI::chooseAction(
    const Player& p,
    const Enemy& e,
    const CombatState& playerState,
    const CombatState& enemyState
) const
{
    switch (e.getPersonality()) {
    case EnemyPersonality::Enraged:
        return chooseEnraged();
    case EnemyPersonality::Subdued:
        return chooseSubdued();
    case EnemyPersonality::Tactical:
        return chooseTactical(playerState);
    case EnemyPersonality::Cautious:
        return chooseCautious(e);
    default:
        return EnemyAction::Attack;
    }
}

// 80% attack
EnemyAction EnemyAI::chooseEnraged() const {
    return roll(1, 100) <= 80 ? EnemyAction::Attack : EnemyAction::Defend;
}

// 80% defend
EnemyAction EnemyAI::chooseSubdued() const {
    return roll(1, 100) <= 80 ? EnemyAction::Defend : EnemyAction::Attack;
}

// Reacts to player patterns
EnemyAction EnemyAI::chooseTactical(const CombatState& playerState) const {
    if (playerState.attacksInRow == 2)
        return EnemyAction::Defend;

    if (playerState.isNextAttackTranquil)
        return EnemyAction::Attack;

    return roll(1, 100) <= 60 ? EnemyAction::Attack : EnemyAction::Defend;
}

// Defends more when low HP
EnemyAction EnemyAI::chooseCautious(const Enemy& e) const {
    const auto& stats = e.getStats();
    float hpPercent = (float)e.current_health / (float)e.max_health;

    if (hpPercent <= 0.30f)
        return roll(1, 100) <= 80 ? EnemyAction::Defend : EnemyAction::Attack;

    return roll(1, 100) <= 60 ? EnemyAction::Attack : EnemyAction::Defend;
}
