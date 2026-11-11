#include "DeathSummaryUI.h"
#include "imgui.h"

void DrawVictorySummaryUI(GameEngine& engine, UIState& uiState)
{
    ImGui::Begin("Victory!");

    Player& player = engine.getPlayer();

    ImGui::Text("You have been victorious in 100 battles!");
    ImGui::Separator();

    ImGui::Text("Final Level: %d", player.getLevel());
    ImGui::Text("Final XP: %d", player.getXP());
    ImGui::Text("Final Gold: %d", player.getGold());


    ImGui::Spacing();

    ImGui::Text("Thanks for playing! -- Feel free to continue");

    ImGui::Spacing();

    if (ImGui::Button("Return to Camp")) {
        engine.enterCamp();
        uiState = UIState::Camp;
    }

    ImGui::End();
}
