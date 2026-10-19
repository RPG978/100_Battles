#pragma once
#include "GameEngine.h"
#include "DatabaseManager.h"

class GameInitializer {
public:
    static bool initialize(GameEngine*& engine, DatabaseManager*& db);
};
