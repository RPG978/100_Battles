#pragma once
#include "GameEngine.h"
#include "Archetype.h"
#include "UIState.h"
#include "ui/MainMenuUI.h"
#include "ui/CombatUI.h"

class UIRouter {
public:
    void draw(GameEngine& engine, UIState& uiState);
};
