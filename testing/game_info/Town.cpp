#include "./Town.h"

Town::Town() {
    faction = Faction::NONE;
}

Town::Town(Faction faction) {
    this->faction = faction;
}

void Town::setFaction(Faction faction) {
    this->faction = faction;
}

Faction Town::getFaction() const {
    return faction;
}