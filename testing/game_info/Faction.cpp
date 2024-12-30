#include "./Faction.h"

std::string factionToString(faction fac) {
    switch (fac) {
        case faction::CASTLE: return "CASTLE";
        case faction::RAMPART: return "RAMPART";
        case faction::TOWER: return "TOWER";
        case faction::INFERNO: return "INFERNO";
        case faction::NECROPOLIS: return "NECROPOLIS";
        case faction::DUNGEON: return "DUNGEON";
        case faction::STRONGHOLD: return "STRONGHOLD";
        case faction::FORTRESS: return "FORTRESS";
        case faction::CONFLUX: return "CONFLUX";
        case faction::NEUTRAL: return "NEUTRAL";
        case faction::NONE: return "NONE";
        case faction::DEFAULT: return "DEFAULT";
    }
    return "DEFAULT";
}