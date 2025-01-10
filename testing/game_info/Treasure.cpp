#include "./Treasure.h"

Treasure::Treasure() : Object() {
    treasureType = TreasureType::RESOURCE_GOLD;
    owner = -1;
}

Treasure::Treasure(TreasureType treasureType) : Object() {
    this->treasureType = treasureType;
    owner = -1;
}

Treasure::Treasure(TreasureType treasureType, int owner, int3 pos, std::string name) : Object(pos, name) {
    this->treasureType = treasureType;
    this->owner = owner;
}

void Treasure::setTreasureType(TreasureType treasureType) {
    this->treasureType = treasureType;
}

void Treasure::setOwner(int owner) {
    this->owner = owner;
}

int Treasure::getOwner() const {
    return owner;
}

TreasureType Treasure::getTreasureType() const {
    return treasureType;
}

