#include "DeathSummaryUI.h"
#include "imgui.h"

void DrawDeathSummaryUI(GameEngine& engine, UIState& uiState)
{
    ImGui::Begin("Defeat");

    Player& player = engine.getPlayer();

    ImGui::Text("You have been defeated...");
    ImGui::Separator();

    ImGui::Text("Lost Gold: %d", engine.getLastLostGold());
    ImGui::Text("Lost XP: %d", engine.getLastLostXP());

    ImGui::Spacing();
    ImGui::Text("You revive at half health.");
    ImGui::ProgressBar(
        static_cast<float>(player.getCurrentHealth()) / player.getMaxHealth(),
        ImVec2(200, 20),
        (std::to_string(player.getCurrentHealth()) + " / " + std::to_string(player.getMaxHealth())).c_str()
    );

    ImGui::Spacing();

    if (ImGui::Button("Return to Camp")) {
        engine.enterCamp();
        uiState = UIState::Camp;
    }

    ImGui::End();
}
