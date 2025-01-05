
#include "./Tile.h"

Tile::Tile() {
    zoneId = -1;
    isBorder = false;
    isGate = false;
    isMiddleGate = false;
    isRoad = false;
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

void Tile::setIsMiddleGate(bool isMiddleGate){
    this->isMiddleGate = isMiddleGate;
}

bool Tile::getIsMiddleGate(){
    return isMiddleGate;
}

void Tile::setNeighbourZones(int zoneId){
    this->neighbourZones.insert(zoneId);
}

std::set<int> Tile::getNeighbourZones(){
    return neighbourZones;
}
