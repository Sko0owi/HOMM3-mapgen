
#include "./Tile.h"

Tile::Tile() {
    zoneId = -1;
    isEdge = false;
    isGate = false;
}

void Tile::setZoneId(int zoneId) {
    this->zoneId = zoneId;
}

int Tile::getZoneId() {
    return zoneId;
}

void Tile::setIsEdge(bool isEdge) {
    this->isEdge = isEdge;
}

bool Tile::getIsEdge() {
    return isEdge;
}

void Tile::setIsGate(bool isGate) {
    this->isGate = isGate;
}

bool Tile::getIsGate() {
    return isGate;
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
