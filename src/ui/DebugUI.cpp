#include "DebugUI.h"
#include "UIState.h"
#include "imgui.h"

void DrawDebugUI(const CombatState& player, const CombatState& enemy)
{
    ImGui::Begin("Debug");
    ImGui::Text("Debug UI is alive");
    ImGui::End();
}
