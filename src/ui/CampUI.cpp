#include "CampUI.h"
#include "imgui.h"
#include "Player.h"

void DrawCampUI(GameEngine& engine, UIState& uiState)
{

    ImGui::Begin("Camp");

    ImGui::Text("You are resting at camp.");
    ImGui::Separator();

    Player& player = engine.getPlayer();

    // --- Healing Section ---
    ImGui::Text("Healing");

    // HP bar
    float hpFraction = static_cast<float>(player.getCurrentHealth()) / player.getMaxHealth();
    ImGui::ProgressBar(hpFraction, ImVec2(200, 20),
        (std::to_string(player.getCurrentHealth()) + " / " + std::to_string(player.getMaxHealth())).c_str());

    // Healing button (disabled if full HP)
    bool atFullHealth = (player.getCurrentHealth() >= player.getMaxHealth());
    if (atFullHealth) {
        ImGui::BeginDisabled();
    }

    if (ImGui::Button("Heal (10 gold)")) {
        if (player.getGold() >= 10) {
            player.spendGold(10);
            player.setCurrentHealth(player.getMaxHealth());
        }
        else {
            // fallback: half heal
            int halfHeal = player.getMaxHealth() / 2;
            player.setCurrentHealth(std::max(player.getCurrentHealth(), halfHeal));
        }
    }
    if (atFullHealth) {
        ImGui::EndDisabled();
    }

    ImGui::Spacing();
    ImGui::Separator();

    // --- Stat Allocation Section ---
    ImGui::Text("Stat Allocation");
    ImGui::Text("Unspent Points: %d", player.getUnspentPoints());

    // Show current stats
    ImGui::Text("Attack: %d", player.getTotalAttack());
    ImGui::Text("Defense: %d", player.getTotalDefense());

    if (ImGui::Button("Add HP (+10)")) {
        player.allocateStatPoint("HP");
    }
    if (ImGui::Button("Add Attack (+1)")) {
        player.allocateStatPoint("Attack");
    }
    if (ImGui::Button("Add Defense (+1)")) {
        player.allocateStatPoint("Defense");
    }

    ImGui::Spacing();
    ImGui::Separator();

    // --- Rewards Display ---
    ImGui::Text("Progression");
    ImGui::Text("Level: %d", player.getLevel());
    ImGui::Text("XP: %d", player.getXP());
    ImGui::Text("Gold: %d", player.getGold());

    ImGui::Spacing();
    ImGui::Separator();

    // --- Shop Placeholder ---
    Stats wStats = engine.currentRotation.weapon->getStats();
    ImGui::Text("Weapon: %s - %dG (HP %d, ATK %d, DEF %d)",
        engine.currentRotation.weapon->getName().c_str(),
        engine.currentRotation.weaponPrice,
        wStats.health,
        wStats.attack,
        wStats.defense);

    if (ImGui::Button("Buy Weapon")) {
        if (engine.purchase(engine.currentRotation.weapon, engine.currentRotation.weaponPrice)) {
            ImGui::Text("Purchased!");
        }
        else {
            ImGui::Text("Not enough gold!");
        }
    }

    Stats aStats = engine.currentRotation.armor->getStats();
    ImGui::Text("Armor: %s - %dG (HP %d, ATK %d, DEF %d)",
        engine.currentRotation.armor->getName().c_str(),
        engine.currentRotation.armorPrice,
        aStats.health,
        aStats.attack,
        aStats.defense);
    if (ImGui::Button("Buy Armor")) {
        if (engine.purchase(engine.currentRotation.armor, engine.currentRotation.armorPrice)) {
            ImGui::Text("Purchased!");
        }
        else {
            ImGui::Text("Not enough gold!");
        }
    }

    Stats acStats = engine.currentRotation.accessory->getStats();
    ImGui::Text("Accessory: %s - %dG (HP %d, ATK %d, DEF %d)",
        engine.currentRotation.accessory->getName().c_str(),
        engine.currentRotation.accessoryPrice,
        acStats.health,
        acStats.attack,
        acStats.defense);
    if (ImGui::Button("Buy Accessory")) {
        if (engine.purchase(engine.currentRotation.accessory, engine.currentRotation.accessoryPrice)) {
            ImGui::Text("Purchased!");
        }
        else {
            ImGui::Text("Not enough gold!");
        }
    }

    ImGui::Spacing();

    // --- Return to Battle ---
    float windowWidth = ImGui::GetWindowSize().x;
    float buttonWidth = 150.0f; // adjust to match your button text
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    if (ImGui::Button("Return to Battle", ImVec2(buttonWidth, 0))) {
        uiState = UIState::ChooseBattleCount;
    }


    ImGui::End();
}