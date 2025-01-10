#pragma once

#include "./TreasureType.h"
#include "./Object.h"


class Treasure : public Object {
public:

    Treasure();
    Treasure(TreasureType treasureType);
    Treasure(TreasureType treasureType, int owner, int3 pos, std::string name);
    Treasure(TreasureType treasureType, int value);

    void setTreasureType(TreasureType treasureType);
    void setOwner(int owner);
    void setValue(int value);

    int getOwner() const;
    int getValue() const;
    TreasureType getTreasureType() const;


private:
    TreasureType treasureType;
    int owner;
    int value;
};