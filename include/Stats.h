#pragma once
#include <iostream>
#include <string>

// hp-att-def
struct Stats {
    int health = 0;
    int attack = 0;
    int defense = 0;
};

// Helper function
void printStats(const Stats& stats);