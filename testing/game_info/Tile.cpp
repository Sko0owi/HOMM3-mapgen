
#include "./Tile.h"

Tile::Tile() {
    zoneId = -1;
}

void Tile::setZoneId(int zoneId) {
    this->zoneId = zoneId;
}

int Tile::getZoneId() {
    return zoneId;
}