#include "CharacterSelectionUI.h"
#include "Archetype.h"
#include "UIState.h"
#include "imgui.h"

void DrawCharacterSelectionUI(GameEngine& engine, UIState& uiState)
{
    ImGui::Begin("Choose Your Character");

    ImGui::Text("Select an archetype:");

    if (ImGui::Button("Warrior"))
    {
        engine.setSelectedRoll(Archetype::Warrior);
        uiState = UIState::ConfirmCharacter;
    }


    if (ImGui::Button("Rogue"))
    {
        engine.setSelectedRoll(Archetype::Rogue);
        uiState = UIState::ConfirmCharacter;
    }

    if (ImGui::Button("Mage"))
    {
        engine.setSelectedRoll(Archetype::Mage);
        uiState = UIState::ConfirmCharacter;
    }

    ImGui::End();
}
