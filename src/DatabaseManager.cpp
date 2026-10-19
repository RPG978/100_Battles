#include "DatabaseManager.h"
#include "Enemy.h"
#include "Item.h"
#include "ItemFactory.h"
#include <sqlite3.h>
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& dbPath) 
	: connection(nullptr) 
{
    int result = sqlite3_open(dbPath.c_str(), reinterpret_cast<sqlite3**>(&connection));

    if (result != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(reinterpret_cast<sqlite3*>(connection)) << "\n";
        connection = nullptr;
    }
}

DatabaseManager::~DatabaseManager()
{
    if (connection != nullptr) {
        sqlite3_close(reinterpret_cast<sqlite3*>(connection));
        connection = nullptr;
    }
}

std::map<int, Enemy> DatabaseManager::loadEnemies()
{
    std::map<int, Enemy> enemies;

    const char* sql =
        "SELECT id,name,base_health,base_attack,base_defense,attack_element,"
        "defense_element, counter_element, rarity, min_level, max_level "
        "FROM enemies;";

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(reinterpret_cast<sqlite3*>(connection), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare enemies query: "
            << sqlite3_errmsg(reinterpret_cast<sqlite3*>(connection)) << "\n";
        return enemies;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = sqlite3_column_type(stmt, 1) == SQLITE_NULL ? "" :
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int base_health = sqlite3_column_int(stmt, 2);
        int base_attack = sqlite3_column_int(stmt, 3);
        int base_defense = sqlite3_column_int(stmt, 4);
        std::string attack_element = sqlite3_column_type(stmt, 5) == SQLITE_NULL ? "" :
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        std::string defense_element = sqlite3_column_type(stmt, 6) == SQLITE_NULL ? "" :
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        std::string counter_element = sqlite3_column_type(stmt, 7) == SQLITE_NULL ? "" :
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        int rarity = sqlite3_column_int(stmt, 8);
        int min_level = sqlite3_column_int(stmt, 9);
        int max_level = sqlite3_column_int(stmt, 10);

        Stats stats;
        stats.health = base_health;
        stats.attack = base_attack;
        stats.defense = base_defense;

        enemies.emplace(id, Enemy(id, name, stats,
            attack_element, defense_element, counter_element,
            rarity, min_level, max_level));
    }

    sqlite3_finalize(stmt);
    return enemies;
}

std::map<int, std::unique_ptr<Item>> DatabaseManager::loadItems()
{
    std::map<int, std::unique_ptr<Item>> items;

    const char* sql =
        "SELECT id, type, name, base_attack, base_defense, base_health, "
        "element, rarity, min_level, max_level "
        "FROM items;";

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(reinterpret_cast<sqlite3*>(connection), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare items query: "
            << sqlite3_errmsg(reinterpret_cast<sqlite3*>(connection)) << "\n";
        return items;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string type = sqlite3_column_type(stmt, 1) == SQLITE_NULL ? "" :
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string name = sqlite3_column_type(stmt, 2) == SQLITE_NULL ? "" :
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        int base_attack = sqlite3_column_int(stmt, 3);
        int base_defense = sqlite3_column_int(stmt, 4);
        int base_health = sqlite3_column_int(stmt, 5);

        std::string element = sqlite3_column_type(stmt, 6) == SQLITE_NULL ? "" :
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        int rarity = sqlite3_column_int(stmt, 7);
        int min_level = sqlite3_column_int(stmt, 8);
        int max_level = sqlite3_column_int(stmt, 9);

        Stats stats;
        stats.attack = base_attack;
        stats.defense = base_defense;
        stats.health = base_health;

        // Use the factory to create the correct subclass
        auto item = ItemFactory::createItem(type, id, name, stats, element, rarity, min_level, max_level);
        items.emplace(id, std::move(item));
    }

    sqlite3_finalize(stmt);
    return items;
}