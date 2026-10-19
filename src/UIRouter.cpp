#include "UIRouter.h"
#include "CharacterSelectionUI.h"
#include "ConfirmCharacterUI.h"
#include "ChooseBattleCountUI.h"
#include "CharacterOverviewUI.h"
#include "CampUI.h"
#include "DeathSummaryUI.h"
#include "imgui.h"

void UIRouter::draw(GameEngine& engine, UIState& uiState)
{
    switch (uiState)
    {
    case UIState::MainMenu:
        DrawMainMenuUI(engine, uiState);
        break;

    case UIState::CharacterSelection:
        DrawCharacterSelectionUI(engine, uiState);
        break;

    case UIState::ConfirmCharacter:
        DrawConfirmCharacterUI(engine, uiState);
        break;

    case UIState::ChooseBattleCount:
        DrawChooseBattleCountUI(engine, uiState);
        break;

    case UIState::Camp:
        DrawCampUI(engine, uiState);
        DrawCharacterOverview(engine);
        break;

    case UIState::InGame:
        if (engine.getState() == GameState::Combat)
        {
            DrawCombatUI(engine);
            DrawDebugUI(engine.getPlayerState(), engine.getEnemyState());
        }
        else if (engine.getState() == GameState::Exploring)
        {
            engine.enterCamp();
            uiState = UIState::Camp;
        }
        else if (engine.getState() == GameState::DeathSummary)
        {
            uiState = UIState::DeathSummary; // forward to summary
        }
        break;

    case UIState::DeathSummary:
        DrawDeathSummaryUI(engine, uiState);
        break;
    }
}
