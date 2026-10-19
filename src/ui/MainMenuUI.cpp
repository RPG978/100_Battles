#include "MainMenuUI.h"
#include "UIState.h"
#include "imgui.h"

void DrawMainMenuUI(GameEngine& engine, UIState& state)
{
    ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("100 Battles");
    ImGui::Separator();

    if (ImGui::Button("New Game")) {
        engine.startNewGame();
        state = UIState::CharacterSelection;
    }

    if (ImGui::Button("Quit")) {
        engine.stop();
    }

    ImGui::End();
}
