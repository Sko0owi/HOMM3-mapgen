#pragma once

#include "./TreasureType.h"
#include "./Object.h"

std::vector<int3> possibleBlockSizes;

void preparePossibleBlockSizes() {

    possibleBlockSizes.push_back(int3(4, 3, 1));
    possibleBlockSizes.push_back(int3(3, 3, 1));
    possibleBlockSizes.push_back(int3(2, 2, 1));
    possibleBlockSizes.push_back(int3(2, 3, 1));
    possibleBlockSizes.push_back(int3(3, 2, 1));
    possibleBlockSizes.push_back(int3(1, 1, 1));

}

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