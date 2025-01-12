#pragma once

#include <string>

enum class Faction {
    CASTLE,
    RAMPART,
    TOWER,
    INFERNO,
    NECROPOLIS,
    DUNGEON,
    STRONGHOLD,
    FORTRESS,
    CONFLUX,
    NEUTRAL,
    NONE,
    DEFAULT
};

std::string factionToString(Faction fac);

Faction stringToFaction(std::string fac);