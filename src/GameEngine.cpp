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

void GameEngine::collectRewards(const Enemy& enemy)
{
    pendingGold += enemy.rewardGold;
    pendingXP += enemy.rewardXP;

    std::cout
        << "[DEBUG] Pending Rewards: "
        << pendingGold
        << " gold, "
        << pendingXP
        << " xp\n";
}

void GameEngine::startBattleStreak(int count)
{
    selectedStreakCount = count;
    streakRemaining = count;

    pendingGold = 0;
    pendingXP = 0;

    state = GameState::Combat;

    startCombat();
}

void GameEngine::startCombat()
{
    currentEnemy = &enemies.at(roll(1, 20));

    std::cout
        << "[DEBUG] Enemy selected: "
        << currentEnemy->getName()
        << " HP="
        << currentEnemy->current_health
        << "/"
        << currentEnemy->max_health
        << "\n";

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

    switch (combatEngine.getCombatResult())
    {
    case CombatResult::Victory:

        player1->incrementBattlesCompleted();

        collectRewards(*currentEnemy);

        if (--streakRemaining > 0)
        {
            startCombat();
        }
        else
        {
          
            player1->addGold(pendingGold * selectedStreakCount);
            player1->addXP(pendingXP * selectedStreakCount);

            pendingGold = 0;
            pendingXP = 0;

            if (selectedStreakCount >= 100) {

                std::cout
                    << "[DEBUG] Victory check fired. BattlesCompleted="
                    << player1->getBattlesCompleted()
                    << "\n";

                state = GameState::VictorySummary;
                uiState = UIState::VictorySummary;
                return;
            }

            state = GameState::Exploring;
            enterCamp();
        }

        break;

    case CombatResult::Escape:

        pendingGold = 0;
        pendingXP = 0;

        state = GameState::Exploring;
        enterCamp();

        break;

    case CombatResult::Death:

        streakRemaining = 0;

        pendingGold = 0;
        pendingXP = 0;

        setLastDeathPenalty(
            combatEngine.getLastLostGold(),
            combatEngine.getLastLostXP());

        state = GameState::DeathSummary;
        uiState = UIState::DeathSummary;

        break;

    case CombatResult::InProgress:
    default:
        break;
    }

    std::cout << "[DEBUG] Combat Result = "
        << static_cast<int>(combatEngine.getCombatResult())
        << "\n";
}

StoreRotation GameEngine::generateStoreRotation(int playerLevel)
{
    StoreRotation rotation;

    auto filterByLevel = [&](auto& pool) {
        std::vector<Item*> eligible;

        for (auto* item : pool) {
            if (item->min_level <= playerLevel &&
                playerLevel <= item->max_level)
            {
                eligible.push_back(item);
            }
        }

        return eligible;
        };

    auto weaponEligible = filterByLevel(weaponPool);
    auto armorEligible = filterByLevel(armorPool);
    auto accEligible = filterByLevel(accessoryPool);

    if (weaponEligible.empty())
    {
        std::cout << "[ERROR] No eligible weapons for level "
            << playerLevel << "\n";
        return rotation;
    }

    if (armorEligible.empty())
    {
        std::cout << "[ERROR] No eligible armor for level "
            << playerLevel << "\n";
        return rotation;
    }

    if (accEligible.empty())
    {
        std::cout << "[ERROR] No eligible accessories for level "
            << playerLevel << "\n";
        return rotation;
    }

    rotation.weapon =
        dynamic_cast<Weapon*>(
            weaponEligible[roll(0, static_cast<int>(weaponEligible.size()) - 1)]);

    rotation.armor =
        dynamic_cast<Armor*>(
            armorEligible[roll(0, static_cast<int>(armorEligible.size()) - 1)]);

    rotation.accessory =
        dynamic_cast<Accessory*>(
            accEligible[roll(0, static_cast<int>(accEligible.size()) - 1)]);

    auto priceFor = [&](Item* item) {
        int basePrice = 20;
        int rarity = std::clamp(item->rarity, 0, 5);

        int price = basePrice * (rarity + 1);
        price += roll(-price / 10, price / 10);

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
