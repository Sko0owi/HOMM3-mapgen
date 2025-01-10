#include "./TreasureType.h"
#include "../types/int3.h"

std::string treasureTypeToString(TreasureType treasureType) {
    switch (treasureType) {
        case TreasureType::RESOURCE_GOLD:
            return "RESOURCE_GOLD";
        case TreasureType::RESOURCE_CRYSTAL:
            return "RESOURCE_CRYSTAL";
        case TreasureType::RESOURCE_GEMS:
            return "RESOURCE_GEMS";
        case TreasureType::RESOURCE_MERCURY:
            return "RESOURCE_MERCURY";
        case TreasureType::RESOURCE_ORE:
            return "RESOURCE_ORE";
        case TreasureType::RESOURCE_SULFUR:
            return "RESOURCE_SULFUR";
        case TreasureType::RESOURCE_WOOD:
            return "RESOURCE_WOOD";
        default:
            return "RESOURCE_GOLD";
    }
    return "RESOURCE_GOLD";
}

TreasureType stringtoTreasureType(std::string treasureType) {
    if (treasureType == "Gold") {
        return TreasureType::RESOURCE_GOLD;
    } else if (treasureType == "Crystal") {
        return TreasureType::RESOURCE_CRYSTAL;
    } else if (treasureType == "Gems") {
        return TreasureType::RESOURCE_GEMS;
    } else if (treasureType == "Mercury") {
        return TreasureType::RESOURCE_MERCURY;
    } else if (treasureType == "Ore") {
        return TreasureType::RESOURCE_ORE;
    } else if (treasureType == "Sulfur") {
        return TreasureType::RESOURCE_SULFUR;
    } else if (treasureType == "Wood") {
        return TreasureType::RESOURCE_WOOD;
    }
    return TreasureType::RESOURCE_GOLD;
}

int3 getTreasureSize(TreasureType TreasureType) {
    switch(TreasureType) {
        case TreasureType::RESOURCE_GOLD:
        case TreasureType::RESOURCE_CRYSTAL:
        case TreasureType::RESOURCE_GEMS:
        case TreasureType::RESOURCE_MERCURY:
        case TreasureType::RESOURCE_ORE:
        case TreasureType::RESOURCE_SULFUR:
        case TreasureType::RESOURCE_WOOD:
            return int3(1, 1, 1);
        default:
            return int3(-1,-1,-1);
    }
    return int3(1,1,1);
}