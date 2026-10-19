#pragma once
#include <string>

enum class Archetype
{
    Warrior,
    Rogue,
    Mage
};

inline std::string ArchetypeToString(Archetype type)
{
    switch (type)
    {
    case Archetype::Warrior: return "Warrior";
    case Archetype::Rogue:   return "Rogue";
    case Archetype::Mage:    return "Mage";
    }
    return "Unknown";
}
