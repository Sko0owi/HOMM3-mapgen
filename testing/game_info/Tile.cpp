
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

// void Tile::setNeighbourZones(int zoneId){
//     this->neighbourZones.insert(zoneId);
// }

// int Tile::getNeighbourZones(){
//     return neighbourZones.size();
// }
