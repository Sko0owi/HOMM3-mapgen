#include <string>

enum class faction {
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

std::string factionToString(faction fac);