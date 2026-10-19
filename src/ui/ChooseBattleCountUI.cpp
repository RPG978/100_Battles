#include "ChooseBattleCountUI.h"
#include "imgui.h"

void DrawChooseBattleCountUI(GameEngine& engine, UIState& uiState)
{
    ImGui::Begin("Battle Streak");

    static int streak = 1;

    ImGui::Text("How many battles?");

    // Larger slider (wider size)
    ImGui::PushItemWidth(300); // make slider wider
    ImGui::SliderInt("##streak_slider", &streak, 1, 100);
    ImGui::PopItemWidth();

    // Text input for precise control
    ImGui::InputInt("##streak_input", &streak);

    // Clamp to valid range
    if (streak < 1) streak = 1;
    if (streak > 100) streak = 100;

    ImGui::Spacing();

    if (ImGui::Button("Start")) {
        engine.startBattleStreak(streak);
        uiState = UIState::InGame;
    }

    ImGui::End();
}
