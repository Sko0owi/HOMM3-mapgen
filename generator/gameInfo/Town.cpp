#include "./Town.h"
#include "../templateInfo/TownInfo.h"

Town::Town() : Object(int3(5,3,1), "Town") {
    faction = Faction::NONE;
}

Town::Town(Faction faction) : Object(int3(5,3,1), "Town") {
    this->faction = faction;
}

Town::Town(Faction faction, int owner, int3 pos, std::string name) : Object(pos, name) {
    this->faction = faction;
    this->owner = owner;
}

Town::Town(TownInfo townInfo) : Object(int3(5,3,1), "Town") {
    this->faction = townInfo.getFaction();
    this->owner = townInfo.getOwner();
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