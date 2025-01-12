#pragma once

#include "./TreasureType.h"
#include "./Object.h"


class Treasure : public Object {
public:

    Treasure();
    Treasure(TreasureType treasureType);
    Treasure(TreasureType treasureType, int owner, int3 pos, std::string name);
    Treasure(TreasureType treasureType, int quantity);

    void setTreasureType(TreasureType treasureType);
    void setOwner(int owner);
    void setQuantity(int quantity);


    int getOwner() const;
    int getQuantity() const;
    TreasureType getTreasureType() const;


private:
    TreasureType treasureType;
    int owner;
    int quantity;
};