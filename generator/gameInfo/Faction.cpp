#include "./Faction.h"

std::string factionToString(Faction fac) {
    switch (fac) {
        case Faction::CASTLE: return "CASTLE";
        case Faction::RAMPART: return "RAMPART";
        case Faction::TOWER: return "TOWER";
        case Faction::INFERNO: return "INFERNO";
        case Faction::NECROPOLIS: return "NECROPOLIS";
        case Faction::DUNGEON: return "DUNGEON";
        case Faction::STRONGHOLD: return "STRONGHOLD";
        case Faction::FORTRESS: return "FORTRESS";
        case Faction::CONFLUX: return "CONFLUX";
        case Faction::NEUTRAL: return "NEUTRAL";
        case Faction::NONE: return "NONE";
        case Faction::DEFAULT: return "DEFAULT";
    }
    return "DEFAULT";
}


Faction stringToFaction(std::string fac) {
    if (fac == "Castle") {
        return Faction::CASTLE;
    } else if (fac == "Rampart") {
        return Faction::RAMPART;
    } else if (fac == "Tower") {
        return Faction::TOWER;
    } else if (fac == "Inferno") {
        return Faction::INFERNO;
    } else if (fac == "Necropolis") {
        return Faction::NECROPOLIS;
    } else if (fac == "Dungeon") {
        return Faction::DUNGEON;
    } else if (fac == "Stronghold") {
        return Faction::STRONGHOLD;
    } else if (fac == "Fortress") {
        return Faction::FORTRESS;
    } else if (fac == "Conflux") {
        return Faction::CONFLUX;
    } else if (fac == "Neutral") {
        return Faction::NEUTRAL;
    } else if (fac == "None") {
        return Faction::NONE;
    }
    return Faction::DEFAULT;
}