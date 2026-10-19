#include <random>
#include "Random.h"

#ifdef UNIT_TEST
static int forcedRoll = -1;

void setForcedRoll(int value) {
    forcedRoll = value;
}

void clearForcedRoll() {
    forcedRoll = -1;
}
#endif

int roll(int min, int max) {
#ifdef UNIT_TEST
    if (forcedRoll >= 0) {
        return forcedRoll; // deterministic override for tests
    }
#endif

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}
