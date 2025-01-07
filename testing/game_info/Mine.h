#pragma once

#include "./MineType.h"
#include "./Object.h"

class Mine : public Object {
public:
    Mine();
    Mine(MineType mineType);

    Mine(MineType mineType, int owner, int3 pos, std::string name);

    void setMineType(MineType mineType);
    void setOwner(int owner);

    int getOwner() const;
    MineType getMineType() const;

private:
    MineType mineType;
    int owner;
};