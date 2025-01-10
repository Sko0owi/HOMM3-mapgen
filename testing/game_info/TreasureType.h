#pragma once

#include<string>

class int3;

enum class TreasureType {
    RESOURCE_GOLD, 
    RESOURCE_CRYSTAL,
    RESOURCE_GEMS,
    RESOURCE_MERCURY,
    RESOURCE_ORE,
    RESOURCE_SULFUR,
    RESOURCE_WOOD,
};

std::string treasureTypeToString(TreasureType treasureType);

TreasureType stringtoTreasureType(std::string treasureType);

int3 getTreasureSize(TreasureType treasureType);