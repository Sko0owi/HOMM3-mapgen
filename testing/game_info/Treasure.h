#pragma once

#include "./TreasureType.h"
#include "./Object.h"


class Treasure : public Object {
public:

    Treasure();
    Treasure(TreasureType treasureType);
    Treasure(TreasureType treasureType, int owner, int3 pos, std::string name);

    void setTreasureType(TreasureType treasureType);
    void setOwner(int owner);

    int getOwner() const;
    TreasureType getTreasureType() const;

private:
    TreasureType treasureType;
    int owner;
};