#include <catch2/catch_test_macros.hpp>
#include "LoggingTestShim.h"
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"

TEST_CASE("Short log messages are stored correctly") {
    testLogLines.clear();
    TestAddLog("You defend!");

    REQUIRE(testLogLines.size() == 1);
    REQUIRE(testLogLines[0] == "You defend!");
}

TEST_CASE("Long log messages do not corrupt") {
    testLogLines.clear();

    std::string longMsg(200, 'A'); // 200 chars
    TestAddLog(longMsg);

    REQUIRE(testLogLines.size() == 1);
    REQUIRE(testLogLines[0] == longMsg);
}

TEST_CASE("Multiple logs remain intact") {
    testLogLines.clear();

    TestAddLog("You defend!");
    TestAddLog("You use an item.");
    TestAddLog("You attack the enemy!");

    REQUIRE(testLogLines[0] == "You defend!");
    REQUIRE(testLogLines[1] == "You use an item.");
    REQUIRE(testLogLines[2] == "You attack the enemy!");
}

TEST_CASE("Stress test logging") {
    testLogLines.clear();

    for (int i = 0; i < 1000; ++i)
        TestAddLog("Log entry " + std::to_string(i));

    REQUIRE(testLogLines.size() == 1000);
    REQUIRE(testLogLines[500] == "Log entry 500");
}
