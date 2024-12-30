#include "./Template.h"

ZoneConnection::ZoneConnection() {
    zoneA = 0;
    zoneB = 0;
}

i32 ZoneConnection::getZoneA() {
    return zoneA;
}

i32 ZoneConnection::getZoneB() {
    return zoneB;
}

void ZoneConnection::setZoneA(i32 zone) {
    zoneA = zone;
}

void ZoneConnection::setZoneB(i32 zone) {
    zoneB = zone;
}

ZoneInfo::ZoneInfo() {
    id = 0;
    size = 0;
    player = 0;
    hero = 0;
}

void ZoneInfo::addConnection(json& connectionConfig) {
    ZoneConnection zoneConnection;
    zoneConnection.setZoneA(connectionConfig["zoneA"]);
    zoneConnection.setZoneB(connectionConfig["zoneB"]);
    connections.push_back(zoneConnection);
}

std::vector<ZoneConnection> ZoneInfo::getConnections() {
    return connections;
}

void ZoneInfo::setId(i32 id) {
    this->id = id;
}

void ZoneInfo::setSize(i32 size) {
    this->size = size;
}

void ZoneInfo::setPlayer(i32 player) {
    this->player = player;
}

void ZoneInfo::setHero(i32 hero) {
    this->hero = hero;
}

i32 ZoneInfo::getId() {
    return id;
}

i32 ZoneInfo::getSize() {
    return size;
}

i32 ZoneInfo::getPlayer() {
    return player;
}

i32 ZoneInfo::getHero() {
    return hero;
}

void ZoneInfo::deserializeZone(const json& config) {
    setId(config["id"]);
    setSize(config["size"]);
    setPlayer(config["player"]);
    setHero(config["hero"]);
    for(auto connectionConfig : config["connections"]) {
        addConnection(connectionConfig);
    }
}

TemplateInfo::Zones & TemplateInfo::getZones() {
    return zones;
}

void TemplateInfo::deserialize(json& config) {
    for (const auto& zoneConfig : config["zones"]) {
        auto zone = std::make_shared<ZoneInfo>();
        zone->deserializeZone(zoneConfig);
        zones[zone->getId()] = zone;
        
    }
}