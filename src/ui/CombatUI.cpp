#include "CombatUI.h"
#include "UIState.h"
#include "imgui.h"
#include <vector>

static std::vector<std::string> logLines;
static std::vector<ImVec4> logColors;

static void AddLog(const std::string& text, ImVec4 color = ImVec4(1, 1, 1, 1))
{
    logLines.push_back(text);
    logColors.push_back(color);
}

void DrawCombatUI(GameEngine& engine)
{
    const CombatState& player = engine.getPlayerState();
    const CombatState& enemy = engine.getEnemyState();

    // === Combat Window ===
    ImGui::Begin("Combat");

    // Player HP bar
    float playerHP = 0.0f;
    if (player.maxHealth > 0)
        playerHP = (float)player.health / (float)player.maxHealth;
    ImGui::Text("Player HP: %d / %d", player.health, player.maxHealth);
    ImGui::ProgressBar(playerHP, ImVec2(-1, 0));

    ImGui::Separator();

    // Enemy HP bar
    float enemyHP = 0.0f;
    if (enemy.maxHealth > 0)
        enemyHP = (float)enemy.health / (float)enemy.maxHealth;
    ImGui::Text("Enemy HP: %d / %d", enemy.health, enemy.maxHealth);
    ImGui::ProgressBar(enemyHP, ImVec2(-1, 0));

    ImGui::Separator();

    // === Action Buttons ===
    if (ImGui::Button("Attack"))
    {
        engine.attack();
        engine.updateCombat();
        AddLog("You attack the enemy!", ImVec4(1.0f, 0.8f, 0.2f, 1));
    }

    ImGui::SameLine();

    if (ImGui::Button("Defend"))
    {
        engine.defend();
        engine.updateCombat();
        AddLog("You defend!", ImVec4(0.4f, 0.7f, 1, 1));
    }

    if (ImGui::Button("Use Item"))
    {
        // Placeholder — later open an inventory window
        engine.useItem("potion");
        engine.updateCombat();
        AddLog("You use an item.", ImVec4(0.6f, 1, 0.6f, 1));
    }

    ImGui::SameLine();

    if (ImGui::Button("Run"))
    {
        engine.tryRun();
        engine.updateCombat();
        AddLog("You attempt to run!", ImVec4(1, 0.4f, 0.4f, 1));
    }

    ImGui::End();

    // === Combat Log Window ===
    ImGui::Begin("Combat Log");

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true);

    // DEBUG: print raw log contents to console
    static int lastPrinted = 0;

    for (int i = lastPrinted; i < logLines.size(); i++)
    {
        const std::string& s = logLines[i];

        std::cout << "[CombatUI] Log[" << i << "] size=" << s.size() << " bytes: ";

        for (unsigned char c : s)
        {
            if (std::isprint(c))
                std::cout << c;
            else
                std::cout << "\\x" << std::hex << (int)c << std::dec;
        }

        std::cout << std::endl;
    }

    lastPrinted = logLines.size();


    for (int i = 0; i < logLines.size(); i++)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, logColors[i]);
        ImGui::TextWrapped("%s", logLines[i].c_str());
        ImGui::PopStyleColor();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}
