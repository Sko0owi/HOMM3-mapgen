#pragma once 

#include "../game_info/MineType.h"

class MineInfo {
public:
    MineInfo();
    MineInfo(MineType mineType);

    MineInfo(MineType mineType, int owner, int minCount);

    void setMineType(MineType mineType);
    void setOwner(int owner);
    void setMinCount(int minCount);

    int getOwner() const;
    MineType getMineType() const;
    int getMinCount() const;


private:
    MineType mineType;
    int owner;
    int minCount;
};
