#include "CharacterOverviewUI.h"
#include "imgui.h"
#include "Player.h"

void DrawCharacterOverview(GameEngine& engine)
{
    Player& player = engine.getPlayer();
        ImGui::Begin("Character Overview");

        // Header with color
        ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), "Character Summary");

        ImGui::Separator();

        // --- Core Stats ---
        Stats totals = player.getTotalStats();
        ImGui::TextColored(ImVec4(0.6f, 1.0f, 0.6f, 1.0f),
            "HP: %d / %d", player.getCurrentHealth(), totals.health);
        ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f),
            "Attack: %d", totals.attack);
        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.6f, 1.0f),
            "Defense: %d", totals.defense);

        ImGui::Separator();

        // --- Progression ---
        ImGui::Text("Level: %d", player.getLevel());
        ImGui::Text("XP: %d", player.getXP());
        ImGui::Text("Gold: %d", player.getGold());

        ImGui::Separator();

        // --- Equipment ---
        ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.4f, 1.0f), "Equipment");

        if (player.getWeapon())
            ImGui::Text("Weapon: %s (HP %d, ATK %d, DEF %d)",
                player.getWeapon()->getName().c_str(),
                player.getWeapon()->getStats().health,
                player.getWeapon()->getStats().attack,
                player.getWeapon()->getStats().defense);
        else
            ImGui::Text("Weapon: None");

        if (player.getArmor())
            ImGui::Text("Armor: %s (HP %d, ATK %d, DEF %d)",
                player.getArmor()->getName().c_str(),
                player.getArmor()->getStats().health,
                player.getArmor()->getStats().attack,
                player.getArmor()->getStats().defense);
        else
            ImGui::Text("Armor: None");

        if (player.getAccessory())
            ImGui::Text("Accessory: %s (HP %d, ATK %d, DEF %d)",
                player.getAccessory()->getName().c_str(),
                player.getAccessory()->getStats().health,
                player.getAccessory()->getStats().attack,
                player.getAccessory()->getStats().defense);
        else
            ImGui::Text("Accessory: None");

        ImGui::End();
}