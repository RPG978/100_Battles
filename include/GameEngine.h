#pragma once
#include "Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "DatabaseManager.h"
#include "CombatEngine.h"
#include "Archetype.h"
#include "ArchetypeData.h"
#include "RolledArchetype.h"
#include "ui/UIState.h"
#include <map>
#include <optional>

enum class GameState {
    MainMenu,
    CharacterSelection,
    StarterEquipment,
    Exploring,
    Camp,
    Combat,
    DeathSummary
};

struct StoreRotation {
    Weapon* weapon;
    int weaponPrice;
    Armor* armor;
    int armorPrice;
    Accessory* accessory;
    int accessoryPrice;
};

class GameEngine {
public:
    GameEngine(DatabaseManager* db);

    // UI-driven lifecycle
    void startNewGame();
    bool isRunning() const { return running; }
    void stop() { running = false; }

    void loadWorld();
    void update();        // per-frame tick
    void startCombat();

    // UI-driven combat actions
    void attack();
    void defend();
    void useItem(const std::string& item);
    void tryRun();

    // UI accessors
    const CombatState& getPlayerState() const { return combatEngine.getPlayerState(); }
    const CombatState& getEnemyState() const { return combatEngine.getEnemyState(); }
    Player& getPlayer() { return *player1; }
    const Player& getPlayer() const { return *player1; }

    GameState getState() const { return state; }
    void enterCamp();

    void chooseArchetype(Archetype type);

    void setSelectedRoll(Archetype type);
    RolledArchetype& getSelectedRoll();

    void startBattleStreak(int count);
    void updateCombat();

    void setLastDeathPenalty(int gold, int xp) {
        lastLostGold = gold;
        lastLostXP = xp;
    }

    int getLastLostGold() const { return lastLostGold; }
    int getLastLostXP() const { return lastLostXP; }

    void setUIState(UIState newState) { uiState = newState; }
    void insertCustomItem(int attack, int defense, int rarity, bool fire, bool ice);

    StoreRotation generateStoreRotation(int playerLevel);
    bool purchase(Item* item, int price);
    StoreRotation currentRotation;

private:
    std::optional<Player> player1;

    Player player2;
    Player player3;

    DatabaseManager* db;
    CombatEngine combatEngine;

    ArchetypeData warriorData;
    ArchetypeData rogueData;
    ArchetypeData mageData;

    std::optional<RolledArchetype> selectedRoll;

    std::map<int, Enemy> enemies;
    std::map<int, std::unique_ptr<Item>> items;
    std::map<int, Weapon> weapons;
    std::map<int, Armor> armors;
    std::map<int, Accessory> accessories;

    Enemy* currentEnemy = nullptr;
    GameState state = GameState::MainMenu;

    bool running = true;
    int streakRemaining = 0;

    int lastLostGold = 0;
    int lastLostXP = 0;
    UIState uiState = UIState::MainMenu;
    
};
