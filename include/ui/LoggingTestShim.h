#pragma once
#include <string>
#include <vector>
#include "imgui.h"

// These mimic the CombatUI.cpp statics
extern std::vector<std::string> testLogLines;
extern std::vector<ImVec4> testLogColors;

inline void TestAddLog(const std::string& text, ImVec4 color = ImVec4(1, 1, 1, 1))
{
    testLogLines.push_back(text);
    testLogColors.push_back(color);
}
