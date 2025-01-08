#include "./MineInfo.h"

MineInfo::MineInfo() {
    mineType = MineType::MINE_SAWMILL;
    owner = 0;
    density = 0;
    minCount = 0;
}

MineInfo::MineInfo(MineType mineType) {
    this->mineType = mineType;
    owner = 0;
    density = 0;
    minCount = 0;
}

MineInfo::MineInfo(MineType mineType, int owner, int density, int minCount) {
    this->mineType = mineType;
    this->owner = owner;
    this->density = density;
    this->minCount = minCount;
}

void MineInfo::setMineType(MineType mineType) {
    this->mineType = mineType;
}

void MineInfo::setOwner(int owner) {
    this->owner = owner;
}

void MineInfo::setDensity(int density) {
    this->density = density;
}

void MineInfo::setMinCount(int minCount) {
    this->minCount = minCount;
}

int MineInfo::getOwner() const {
    return owner;
}

MineType MineInfo::getMineType() const {
    return mineType;
}

int MineInfo::getDensity() const {
    return density;
}

int MineInfo::getMinCount() const {
    return minCount;
}


