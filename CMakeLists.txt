cmake_minimum_required(VERSION 3.16)
project(100_Battles)

set(CMAKE_CXX_STANDARD 20)

# -------------------------------
# Game sources
# -------------------------------
add_library(game
    src/sqlite3.c
    src/Stats.cpp
    src/StoreEngine.cpp
    src/UIRouter.cpp
    src/Weapon.cpp
    src/Accessory.cpp
    src/Armor.cpp
    src/CharacterSelectionUI.cpp
    src/CombatEngine.cpp
    src/DatabaseManager.cpp
    src/EncounterTable.cpp
    src/Enemy.cpp
    src/EnemyAI.cpp
    src/GameEngine.cpp
    src/GameInitializer.cpp
    src/ItemFactory.cpp
    src/LevelingSystem.cpp
    src/MessageFormatter.cpp
    src/Player.cpp
    src/Random.cpp
    # UI implementations
    src/ui/ImGuiLayer.cpp
    src/ui/MainMenuUI.cpp
    src/ui/CombatUI.cpp
    src/ui/DebugUI.cpp
    src/ui/ConfirmCharacterUI.cpp
    src/ui/ChooseBattleCountUI.cpp
    src/ui/CampUI.cpp
    src/ui/CharacterOverviewUI.cpp
    src/ui/DeathSummaryUI.cpp
    src/ui/LoggingTestShim.cpp
)

target_include_directories(game PUBLIC
    include
    include/ui
    external
    external/imgui
    external/imgui/backends
)

# -------------------------------
# ImGui sources (core + backends)
# -------------------------------
set(IMGUI_SRC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends)

# -------------------------------
# Catch2 for tests
# -------------------------------
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(catch2)

add_executable(tests
    tests/test_combat.cpp
    tests/test_logging.cpp
    tests/test_ai.cpp
    src/Random.cpp
)

target_include_directories(tests PRIVATE include)
# tests do NOT link SDL2main
target_link_libraries(tests PRIVATE game imgui Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)

target_compile_definitions(tests PRIVATE UNIT_TEST)

# -------------------------------
# Game executable
# -------------------------------
add_executable(game_main ${CMAKE_SOURCE_DIR}/main.cpp)
target_link_libraries(game_main PRIVATE game imgui SDL2::SDL2 OpenGL::GL)

# SDL2 + OpenGL via vcpkg
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Link SDL2 core + OpenGL only
target_link_libraries(game PUBLIC SDL2::SDL2 OpenGL::GL)
target_link_libraries(imgui PUBLIC SDL2::SDL2 OpenGL::GL)
target_link_libraries(game_main PRIVATE game imgui SDL2::SDL2 OpenGL::GL)
