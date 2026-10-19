#include "Stats.h"

void printStats(const Stats& stats) {
    std::cout << "Attack:  " << stats.attack << "\n"
        << "Defense: " << stats.defense << "\n"
        << "Health:  " << stats.health << "\n";
}
