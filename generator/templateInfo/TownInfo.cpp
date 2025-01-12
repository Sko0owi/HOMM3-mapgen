#include "./TownInfo.h"

TownInfo::TownInfo() {
    faction = Faction::NONE;
    owner = 0;
    density = 0;
    minCount = 0;
}

TownInfo::TownInfo(Faction faction) {
    this->faction = faction;
    owner = 0;
    density = 0;
    minCount = 0;
}

TownInfo::TownInfo(Faction faction, int owner) {
    this->faction = faction;
    this->owner = owner;
    density = 0;
    minCount = 0;
}

TownInfo::TownInfo(Faction faction, int owner, int density, int minCount) {
    this->faction = faction;
    this->owner = owner;
    this->density = density;
    this->minCount = minCount;
}

void TownInfo::setFaction(Faction faction) {
    this->faction = faction;
}

void TownInfo::setOwner(int owner) {
    this->owner = owner;
}

void TownInfo::setDensity(int density) {
    this->density = density;
}

void TownInfo::setMinCount(int minCount) {
    this->minCount = minCount;
}

int TownInfo::getOwner() const {
    return owner;
}

Faction TownInfo::getFaction() const {
    return faction;
}

int TownInfo::getDensity() const {
    return density;
}

int TownInfo::getMinCount() const {
    return minCount;
}

