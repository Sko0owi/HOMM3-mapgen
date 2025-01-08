#pragma once 

#include "../game_info/MineType.h"

class MineInfo {
public:
    MineInfo();
    MineInfo(MineType mineType);

    MineInfo(MineType mineType, int owner, int density, int minCount);

    void setMineType(MineType mineType);
    void setOwner(int owner);
    void setDensity(int density);
    void setMinCount(int minCount);

    int getOwner() const;
    MineType getMineType() const;
    int getDensity() const;
    int getMinCount() const;


private:
    MineType mineType;
    int owner;
    int density;
    int minCount;
};
