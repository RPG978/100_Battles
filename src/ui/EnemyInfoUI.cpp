#include "EnemyInfoUI.h"
#include "UIState.h"
#include "imgui.h"
#include "Enemy.h"
#include <string>

namespace
{
    std::string GetRarityName(int rarity)
    {
        switch (rarity)
        {
        case 1: return "Common";
        case 2: return "Uncommon";
        case 3: return "Rare";
        case 4: return "Epic";
        case 5: return "Legendary";
        default: return "Unknown";
        }
    }

    std::string GetWeakness(const std::string& element)
    {
        if (element == "Fire")      return "Water";
        if (element == "Water")     return "Lightning";
        if (element == "Lightning") return "Earth";
        if (element == "Earth")     return "Poison";
        if (element == "Poison")    return "Fire";

        return "None";
    }

    std::string GetStrength(const std::string& element)
    {
        if (element == "Fire")      return "Poison";
        if (element == "Water")     return "Fire";
        if (element == "Lightning") return "Water";
        if (element == "Earth")     return "Lightning";
        if (element == "Poison")    return "Earth";

        return "None";
    }
}

void DrawEnemyInfoUI(GameEngine& engine)
{
    const Enemy* enemy = engine.getCurrentEnemy();

    if (!enemy)
        return;

    ImGui::Begin("Enemy Info");

    ImGui::Text("%s", enemy->getName().c_str());

    ImGui::Text("%s",
        GetRarityName(enemy->getRarity()).c_str());

    ImGui::Separator();

    ImGui::Text("Attack: %d",
        enemy->getStats().attack);

    ImGui::Text("Defense: %d",
        enemy->getStats().defense);

    ImGui::Separator();

    ImGui::Text("Attack Element: %s",
        enemy->getAttackElement().c_str());

    ImGui::Text("Defense Element: %s",
        enemy->getDefenseElement().c_str());

    ImGui::Text("Counter Element: %s",
        enemy->getCounterElement().c_str());

    ImGui::Separator();

    ImGui::Text("Weak To: %s",
        GetWeakness(
            enemy->getDefenseElement()).c_str());

    ImGui::Text("Strong Vs: %s",
        GetStrength(
            enemy->getAttackElement()).c_str());

    ImGui::End();
}