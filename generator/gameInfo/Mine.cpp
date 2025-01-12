#include "./Mine.h"
#include "../templateInfo/MineInfo.h"

Mine::Mine() {
    mineType = MineType::MINE_SAWMILL;
}

Mine::Mine(MineType mineType) {
    this->mineType = mineType;
}

Mine::Mine(MineType mineType, int owner, int3 pos, std::string name) : Object(pos, name) {
    this->mineType = mineType;
    this->owner = owner;
}

Mine::Mine(MineInfo mineInfo) : Object(int3(0,0,0), "Mine") {
    this->mineType = mineInfo.getMineType();
    this->owner = mineInfo.getOwner();
}

void Mine::setMineType(MineType mineType) {
    this->mineType = mineType;
}

MineType Mine::getMineType() const {
    return mineType;
}

void Mine::setOwner(int owner) {
    this->owner = owner;
}

int Mine::getOwner() const {
    return owner;
}

