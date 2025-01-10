#pragma once

#include<string>

class int3;

enum TreasureType {
    LOW_TIER_TAB,

    RESOURCE_WOOD,
    RESOURCE_ORE,

    MID_TIER_TAB,

    RESOURCE_SULFUR,
    RESOURCE_CRYSTAL,
    RESOURCE_GEMS,
    RESOURCE_MERCURY,

    HIGH_TIER_TAB,

    RESOURCE_GOLD, 
    
    END_TIER_TAB
};


std::string treasureTypeToString(TreasureType treasureType);

TreasureType stringtoTreasureType(std::string treasureType);

int3 getTreasureSize(TreasureType treasureType);