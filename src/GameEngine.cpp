#include "GameEngine.h"
#include "Random.h"
#include "MessageFormatter.h"
#include <iostream>
#include <algorithm>
#include <vector>

std::optional<RolledArchetype> warriorRoll;
std::optional<RolledArchetype> rogueRoll;
std::optional<RolledArchetype> mageRoll;

std::vector<Weapon*> weaponPool;
std::vector<Armor*> armorPool;
std::vector<Accessory*> accessoryPool;

GameEngine::GameEngine(DatabaseManager* db)
    : db(db)
{
    warriorData = {
        Stats{ roll(200,260), roll(20,40), roll(20,30) },
        1, 
        1, 
        1  
    };

    rogueData = {
        Stats{ roll(140,200), roll(30,50), roll(30,40) },
        2, 
        2, 
        2  
    };

    mageData = {
        Stats{ roll(170,230), roll(40,60), roll(10,20) },
        3, 
        3, 
        3  
    };
}

void GameEngine::loadWorld()
{
    enemies = db->loadEnemies();
    items = std::move(db->loadItems());
    
    weaponPool.clear();
    armorPool.clear();
    accessoryPool.clear();

    for (auto& [id, ptr] : items) {
        if (auto w = dynamic_cast<Weapon*>(ptr.get())) weaponPool.push_back(w);
        else if (auto a = dynamic_cast<Armor*>(ptr.get())) armorPool.push_back(a);
        else if (auto ac = dynamic_cast<Accessory*>(ptr.get())) accessoryPool.push_back(ac);
    }
}

void GameEngine::startNewGame()
{
    currentEnemy = nullptr;
    combatEngine.reset();

    // No player yet — user will choose one
    player1 = std::nullopt;

    warriorRoll = RolledArchetype{
        Archetype::Warrior,
        Stats{ roll(200,260), roll(20,40), roll(20,30) }
    };

    rogueRoll = RolledArchetype{
        Archetype::Rogue,
        Stats{ roll(140,200), roll(30,50), roll(30,40) }
    };

    mageRoll = RolledArchetype{
        Archetype::Mage,
        Stats{ roll(170,230), roll(40,60), roll(10,20) }
    };

    state = GameState::CharacterSelection;
}

void GameEngine::chooseArchetype(Archetype type)
{
    std::cout << "[DEBUG] chooseArchetype called with type=" << ArchetypeToString(type) << "\n";

    ArchetypeData* data = nullptr;

    switch (type)
    {
    case Archetype::Warrior: data = &warriorData; break;
    case Archetype::Rogue:   data = &rogueData;   break;
    case Archetype::Mage:    data = &mageData;    break;
    }

    player1 = Player("Player", data->baseStats);
    player1->setGold(50);

    std::cout << "[DEBUG] Player created with HP=" << player1->getCurrentHealth()
        << " Gold=" << player1->getGold() << "\n";

    state = GameState::Exploring;
    std::cout << "[DEBUG] GameState set to Exploring\n";
}

void GameEngine::update()
{
    // Per-frame logic goes here.
    // For now, nothing happens automatically.
}

void GameEngine::enterCamp()
{

    state = GameState::Exploring;
    uiState = UIState::Camp;

    if (player1) {
        currentRotation = generateStoreRotation(player1->getLevel());
    }
    else {
        std::cout << "[DEBUG] No player1 found!\n";
    }
}

void GameEngine::startBattleStreak(int count)
{
    streakRemaining = count;   // store this in GameEngine as an int
    state = GameState::Combat;

    // Start first battle
    startCombat();
}

void GameEngine::startCombat()
{
    currentEnemy = &enemies.at(roll(1, 20));
    state = GameState::Combat;

    combatEngine.start(*player1, *currentEnemy);
}

void GameEngine::attack()
{
    if (currentEnemy)
        combatEngine.playerAttack(*player1, *currentEnemy);
}

void GameEngine::defend()
{
    if (currentEnemy)
        combatEngine.playerDefend(*player1, *currentEnemy);
}

void GameEngine::useItem(const std::string& item)
{
    if (currentEnemy)
        combatEngine.useItem(*player1, item);
}

void GameEngine::tryRun()
{
    if (currentEnemy)
        combatEngine.tryRun(*player1, *currentEnemy);
}

void GameEngine::setSelectedRoll(Archetype type)
{
    switch (type)
    {
    case Archetype::Warrior: selectedRoll = warriorRoll; break;
    case Archetype::Rogue:   selectedRoll = rogueRoll;   break;
    case Archetype::Mage:    selectedRoll = mageRoll;    break;
    }
}

RolledArchetype& GameEngine::getSelectedRoll()
{
    return *selectedRoll;
}

void GameEngine::updateCombat()
{
    if (!combatEngine.isCombatOver())
        return;

    // Player defeated: streak ends immediately
    if (combatEngine.playerDied())
    {
        std::cout << "[DEBUG] Player died. Ending streak.\n";
        streakRemaining = 0;

        // Capture penalty values from CombatEngine
        setLastDeathPenalty(combatEngine.getLastLostGold(),
            combatEngine.getLastLostXP());

        // Transition to death summary popup instead of Exploring
        state = GameState::DeathSummary;
        uiState = UIState::DeathSummary;
        return;
    }

    // Enemy defeated: continue streak if any remain
    if (--streakRemaining > 0)
    {
        std::cout << "[DEBUG] Enemy defeated. Starting next battle.\n";
        startCombat();
    }
    else
    {
        std::cout << "[DEBUG] Streak complete. Returning to camp.\n";
        state = GameState::Exploring;
        enterCamp();
    }
}

StoreRotation GameEngine::generateStoreRotation(int playerLevel)
{
    StoreRotation rotation;

    // Filter by level range
    auto filterByLevel = [&](auto& pool) {
        std::vector<Item*> eligible;
        for (auto* item : pool) {
            if (item->min_level <= playerLevel && playerLevel <= item->max_level)
                eligible.push_back(item);
        }
        return eligible;
        };

    auto weaponEligible = filterByLevel(weaponPool);
    auto armorEligible = filterByLevel(armorPool);
    auto accEligible = filterByLevel(accessoryPool);

    // Random pick
    rotation.weapon = dynamic_cast<Weapon*>(weaponEligible[roll(0, weaponEligible.size() - 1)]);
    rotation.armor = dynamic_cast<Armor*>(armorEligible[roll(0, armorEligible.size() - 1)]);
    rotation.accessory = dynamic_cast<Accessory*>(accEligible[roll(0, accEligible.size() - 1)]);

    // Price assignment
    auto priceFor = [&](Item* item) {
        int basePrice = 20;
        int rarity = std::clamp(item->rarity, 0, 5);
        int price = basePrice * (rarity + 1);
        price += roll(-price / 10, price / 10); // ±10% variance
        return price;
        };

    rotation.weaponPrice = priceFor(rotation.weapon);
    rotation.armorPrice = priceFor(rotation.armor);
    rotation.accessoryPrice = priceFor(rotation.accessory);

    return rotation;
}

bool GameEngine::purchase(Item* item, int price)
{
    if (!player1) return false;

    if (player1->getGold() >= price) {
        player1->spendGold(price);
        // Equip based on type
        if (auto w = dynamic_cast<Weapon*>(item)) player1->equipWeapon(w);
        else if (auto a = dynamic_cast<Armor*>(item)) player1->equipArmor(a);
        else if (auto ac = dynamic_cast<Accessory*>(item)) player1->equipAccessory(ac);

        Stats totals = player1->getTotalStats();
        player1->recalculateHealth();

        return true; // purchase succeeded
    }
    return false; // not enough gold
}

void GameEngine::insertCustomItem(int attack, int defense, int rarity, bool fire, bool ice)
{
    std::string sql = "INSERT INTO items (attack, defense, rarity, fire, ice) VALUES (?, ?, ?, ?, ?);";
    // Use sqlite3_bind_* to insert values
}
