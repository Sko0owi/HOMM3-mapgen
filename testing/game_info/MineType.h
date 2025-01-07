#include<string>

class int3;

enum class MineType {
    MINE_ALCHEMISTS_LAB = 0,
    MINE_CRYSTAL_CAVERN,
    MINE_GEM_POND,
    MINE_GOLD_MINE,
    MINE_ORE_PIT,
    MINE_SAWMILL,
    MINE_SULFUR_DUNE
};

std::string mineTypeToString(MineType mineType);

MineType stringToMineType(std::string mineType);

int3 getMineSize(MineType mineType);