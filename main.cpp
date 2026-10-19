#include "Enemy.h"
#include "DatabaseManager.h"
#include "GameEngine.h"

#include "GameInitializer.h"
#include "UIRouter.h"
#include "ui/ImGuiLayer.h"

#include <iostream>
#include <ctime>

int main()
{
    try {
        // Seeds random number generator
        srand(static_cast<unsigned>(time(nullptr)));

        // Initialize core systems
        DatabaseManager* db = nullptr;
        GameEngine* engine = nullptr;

        if (!GameInitializer::initialize(engine, db))
            return 1;

        ImGuiLayer ui;
        if (!ui.init())
            return 1;

        UIState uiState = UIState::MainMenu;
        UIRouter router;

        while (engine->isRunning())
        {
            // Starts new ImGui frame
            ui.beginFrame();
            // Chooses an draws UI panels
            router.draw(*engine, uiState);
            // Per-frame game logic
            engine->update();
            // Renders UI
            ui.endFrame();
        }

        // Shut everything down cleanly
        ui.shutdown();
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "An unknown error occurred.\n";
        return 1;
    }
}
