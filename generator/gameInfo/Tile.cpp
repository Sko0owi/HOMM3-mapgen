
#include "./Tile.h"

Tile::Tile() {
    zoneId = -1;
    tier = 0;
    isBorder = false;
    isGate = false;
    isRoad = false;
    isGuard = false;
}

void Tile::setZoneId(int zoneId) {
    this->zoneId = zoneId;
}

int Tile::getZoneId() {
    return zoneId;
}

void Tile::setIsBorder(bool isBorder) {
    this->isBorder = isBorder;
}

bool Tile::getIsBorder() {
    return isBorder;
}

void Tile::setIsGate(bool isGate) {
    this->isGate = isGate;
}

bool Tile::getIsGate() {
    return isGate;
}

void Tile::setIsRoad(bool isRoad) {
    this->isRoad = isRoad;
}

bool Tile::getIsRoad() {
    return isRoad;
}

void Tile::setIsGuard(bool isGuard) {
    this->isGuard = isGuard;
}

bool Tile::getIsGuard() {
    return isGuard;
}

void Tile::setNeighbourZones(int zoneId){
    this->neighbourZones.insert(zoneId);
}

int Tile::getTier() {
    return tier;
}

void Tile::setTier(int tier){
    this->tier = tier;
}

std::set<int> Tile::getNeighbourZones(){
    return neighbourZones;
}
