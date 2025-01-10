#include "./Treasure.h"


Treasure::Treasure() : Object() {
    treasureType = TreasureType::RESOURCE_GOLD;
    owner = -1;
}

Treasure::Treasure(TreasureType treasureType) : Object() {
    this->treasureType = treasureType;
    owner = -1;
}

Treasure::Treasure(TreasureType treasureType, int quantity) : Object(int3(0,0,0), "Treasure") {
    this->treasureType = treasureType;
    this->quantity = quantity;
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

void Treasure::setQuantity(int quantity) {
    this->quantity = quantity;
}

int Treasure::getQuantity() const {
    return quantity;
}

int Treasure::getOwner() const {
    return owner;
}

TreasureType Treasure::getTreasureType() const {
    return treasureType;
}

