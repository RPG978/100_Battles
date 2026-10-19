#pragma once
#include <string>
#include <map>
#include "Enemy.h"

class DatabaseManager {
public:
	DatabaseManager(const std::string& dbPath);

	~DatabaseManager();

	std::map<int, Enemy> loadEnemies();
	std::map<int, std::unique_ptr<Item>> loadItems();

private:
	void* connection;
};