
#include "./MineType.h"
#include "../types/int3.h"

std::string mineTypeToString(MineType mineType) {
    switch (mineType) {
        case MineType::MINE_ALCHEMISTS_LAB:
            return "MINE_ALCHEMISTS_LAB";
        case MineType::MINE_CRYSTAL_CAVERN:
            return "MINE_CRYSTAL_CAVERN";
        case MineType::MINE_GEM_POND:
            return "MINE_GEM_POND";
        case MineType::MINE_GOLD_MINE:
            return "MINE_GOLD_MINE";
        case MineType::MINE_ORE_PIT:
            return "MINE_ORE_PIT";
        case MineType::MINE_SAWMILL:
            return "MINE_SAWMILL";
        case MineType::MINE_SULFUR_DUNE:
            return "MINE_SULFUR_DUNE";
    }
    return "MINE_SAWMILL";
}

MineType stringToMineType(std::string mineType) {
    if (mineType == "Alchemists Lab") return MineType::MINE_ALCHEMISTS_LAB;
    if (mineType == "Crystal Cavern") return MineType::MINE_CRYSTAL_CAVERN;
    if (mineType == "Gem Pond") return MineType::MINE_GEM_POND;
    if (mineType == "Gold Mine") return MineType::MINE_GOLD_MINE;
    if (mineType == "Ore Pit") return MineType::MINE_ORE_PIT;
    if (mineType == "Sawmill") return MineType::MINE_SAWMILL;
    if (mineType == "Sulfur Dune") return MineType::MINE_SULFUR_DUNE;
    return MineType::MINE_SAWMILL;
}

int3 getMineSize(MineType mineType) {
    switch(mineType) {
        case MineType::MINE_ALCHEMISTS_LAB:
            return int3(3,1,1);
        case MineType::MINE_CRYSTAL_CAVERN:
            return int3(3,1,1);
        case MineType::MINE_GEM_POND:
            return int3(3,2,1);
        case MineType::MINE_GOLD_MINE:
            return int3(3,1,1);
        case MineType::MINE_ORE_PIT:
            return int3(3,2,1);
        case MineType::MINE_SAWMILL:
            return int3(4,2,1);
        case MineType::MINE_SULFUR_DUNE:
            return int3(3,1,1);
    }
    return int3(1,1,1);
}