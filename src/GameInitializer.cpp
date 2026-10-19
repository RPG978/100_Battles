#include "GameInitializer.h"

bool GameInitializer::initialize(GameEngine*& engine, DatabaseManager*& db)
{
    db = new DatabaseManager("data/game.db");
    engine = new GameEngine(db);
    engine->loadWorld();
    return true;
}
