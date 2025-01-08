#include "./ConnectionInfo.h"

ConnectionInfo::ConnectionInfo() {
    zoneA = 0;
    zoneB = 0;
    tier = 0;
    type = "narrow";
}

i32 ConnectionInfo::getZoneA() {
    return zoneA;
}

i32 ConnectionInfo::getZoneB() {
    return zoneB;
}

i32 ConnectionInfo::getTier() {
    return tier;
}

string ConnectionInfo::getType() {
    return type;
}

i32 ConnectionInfo::getOtherZone(i32 zone) {
    if (zone == zoneA)
    {
        return zoneB;
    }
    return zoneA;
}

void ConnectionInfo::setZoneA(i32 zone) {
    zoneA = zone;
}

void ConnectionInfo::setZoneB(i32 zone) {
    zoneB = zone;
}

void ConnectionInfo::setTier(i32 tier) {
    this->tier = tier;
}

void ConnectionInfo::setType(string type) {
    this->type = type;
}
