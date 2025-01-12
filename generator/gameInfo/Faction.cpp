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
    if (fac == "TOWN_CASTLE") {
        return Faction::CASTLE;
    } else if (fac == "TOWN_RAMPART") {
        return Faction::RAMPART;
    } else if (fac == "TOWN_TOWER") {
        return Faction::TOWER;
    } else if (fac == "TOWN_INFERNO") {
        return Faction::INFERNO;
    } else if (fac == "TOWN_NECROPOLIS") {
        return Faction::NECROPOLIS;
    } else if (fac == "TOWN_DUNGEON") {
        return Faction::DUNGEON;
    } else if (fac == "TOWN_STRONGHOLD") {
        return Faction::STRONGHOLD;
    } else if (fac == "TOWN_FORTRESS") {
        return Faction::FORTRESS;
    } else if (fac == "TOWN_CONFLUX") {
        return Faction::CONFLUX;
    } else if (fac == "TOWN_NEUTRAL") {
        return Faction::NEUTRAL;
    } else if (fac == "TOWN_NONE") {
        return Faction::NONE;
    }
    return Faction::DEFAULT;
}