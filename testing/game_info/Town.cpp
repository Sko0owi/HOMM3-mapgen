#include "./Town.h"

Town::Town() {
    faction = Faction::NONE;
}

Town::Town(Faction faction) {
    this->faction = faction;
}

Town::Town(Faction faction, int owner, int3 pos, std::string name) : Object(pos, name) {
    this->faction = faction;
    this->owner = owner;
}

void Town::setFaction(Faction faction) {
    this->faction = faction;
}

Faction Town::getFaction() const {
    return faction;
}

void Town::setOwner(int owner) {
    this->owner = owner;
}

int Town::getOwner() const {
    return owner;
}