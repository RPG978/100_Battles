#include "MessageFormatter.h"
#include <iostream>

void MessageFormatter::printBanner() {
    std::cout << "\n=== Main Menu ===\n";
    std::cout << "Commands:\n";
    std::cout << "  (s) Stats   - View player stats\n";
    std::cout << "  (f) Fight   - Start a battle\n";
    std::cout << "  (q) Quit    - Exit the game\n";
}
