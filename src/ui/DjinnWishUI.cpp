#include "DjinnWishUI.h"
#include "imgui.h"

void DrawDjinnWishUI(GameEngine& engine, UIState& uiState)
{
    static int attack = 0;
    static int defense = 0;
    static int rarity = 1;
    static bool elementalFire = false;
    static bool elementalIce = false;

    ImGui::Begin("Djinn's Wish");

    ImGui::InputInt("Attack", &attack);
    ImGui::InputInt("Defense", &defense);
    ImGui::InputInt("Rarity", &rarity);
    ImGui::Checkbox("Fire Element", &elementalFire);
    ImGui::Checkbox("Ice Element", &elementalIce);

    if (ImGui::Button("Save Item")) {
        engine.insertCustomItem(attack, defense, rarity, elementalFire, elementalIce);
        engine.enterCamp();
        uiState = UIState::Camp;
    }

    ImGui::End();
}
