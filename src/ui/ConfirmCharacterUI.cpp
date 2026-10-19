#include "ConfirmCharacterUI.h"
#include "UIState.h"
#include "imgui.h"

void DrawConfirmCharacterUI(GameEngine& engine, UIState& uiState)
{
    ImGui::Begin("Confirm Character");

    // Show the selected archetype + rolled stats
    auto& roll = engine.getSelectedRoll();  

    ImGui::Text("Archetype: %s", ArchetypeToString(roll.type).c_str());

    ImGui::Separator();

    ImGui::Text("HP: %d", roll.rolledStats.health);
    ImGui::Text("ATK: %d", roll.rolledStats.attack);
    ImGui::Text("DEF: %d", roll.rolledStats.defense);

    ImGui::Separator();

    if (ImGui::Button("Confirm"))
    {
        engine.chooseArchetype(roll.type);
        engine.enterCamp();
        uiState = UIState::Camp;
    }

    if (ImGui::Button("Back"))
    {
        uiState = UIState::CharacterSelection;
    }

    ImGui::End();
}
