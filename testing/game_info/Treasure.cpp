#include "./Treasure.h"

Treasure::Treasure() : Object() {
    treasureType = TreasureType::RESOURCE_GOLD;
    owner = -1;
}

Treasure::Treasure(TreasureType treasureType) : Object() {
    this->treasureType = treasureType;
    owner = -1;
}

Treasure::Treasure(TreasureType treasureType, int value) : Object() {
    this->treasureType = treasureType;
    this->value = value;
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

void Treasure::setValue(int value) {
    this->value = value;
}

int Treasure::getValue() const {
    return value;
}

int Treasure::getOwner() const {
    return owner;
}

TreasureType Treasure::getTreasureType() const {
    return treasureType;
}

