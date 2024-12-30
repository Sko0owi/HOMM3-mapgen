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
    size = "";
    player = "";
    hero = "";
}

void ZoneInfo::addConnection(const json& connectionConfig) {
    ZoneConnection zoneConnection;
    int zoneA = connectionConfig["zoneA"].get<int>();
    int zoneB = connectionConfig["zoneB"].get<int>();
    zoneConnection.setZoneA(zoneA);
    zoneConnection.setZoneB(zoneB);
    connections.push_back(zoneConnection);
}

std::vector<ZoneConnection> ZoneInfo::getConnections() {
    return connections;
}

void ZoneInfo::setId(i32 id) {
    this->id = id;
}

void ZoneInfo::setSize(std::string size) {
    this->size = size;
}

void ZoneInfo::setPlayer(std::string player) {
    this->player = player;
}

void ZoneInfo::setHero(std::string hero) {
    this->hero = hero;
}

i32 ZoneInfo::getId() {
    return id;
}

std::string ZoneInfo::getSize() {
    return size;
}

std::string ZoneInfo::getPlayer() {
    return player;
}

std::string ZoneInfo::getHero() {
    return hero;
}

void ZoneInfo::deserializeZone(const json& config) {

    i32 id = config["id"].get<int>();
    std::string size = config["size"].get<std::string>();
    std::string player = config["player"].get<std::string>();
    std::string hero = config["hero"].get<std::string>();
    

    setId(id);
    setSize(size);
    setPlayer(player);
    setHero(hero);
    
}

void ZoneInfo::printZone() {
    std::cerr << "Zone id: " << id << "\n";
    std::cerr << "Zone size: " << size << "\n";
    std::cerr << "Zone player: " << player << "\n";
    std::cerr << "Zone hero: " << hero << "\n";
    for(auto connection : connections) {
        std::cerr << "Connection: \n" << \
        "ZoneA: " << connection.getZoneA() << " " << \
        "ZoneB: " << connection.getZoneB() << "\n";
    }
}

TemplateInfo::TemplateInfo() {
    name = "";
    description = "";
    mapSize = "";
    difficulty = "";
}

ZonesI & TemplateInfo::getZonesI() {
    return zonesI;
}

void TemplateInfo::setName(std::string name) {
    this->name = name;
}

void TemplateInfo::setDescription(std::string description) {
    this->description = description;
}

void TemplateInfo::setMapSize(std::string mapSize) {
    this->mapSize = mapSize;
}

void TemplateInfo::setDifficulty(std::string difficulty) {
    this->difficulty = difficulty;
}

std::string TemplateInfo::getName() {
    return name;
}

std::string TemplateInfo::getDescription() {
    return description;
}

std::string TemplateInfo::getMapSize() {
    return mapSize;
}

std::string TemplateInfo::getDifficulty() {
    return difficulty;
}


void TemplateInfo::deserialize(const json& config) {

    std::string name = config["name"].get<std::string>();
    std::string description = config["description"].get<std::string>();
    std::string mapSize = config["size"].get<std::string>();
    std::string difficulty = config["difficulty"].get<std::string>();

    setName(name);
    setDescription(description);
    setMapSize(mapSize);
    setDifficulty(difficulty);

    for (const auto& zoneConfig : config["zones"]) {
        auto zone = std::make_shared<ZoneInfo>();
        zone->deserializeZone(zoneConfig);
        zonesI[zone->getId()] = zone;
        
    }

    for (const auto& connectionConfig : config["connections"]) {
        i32 zoneA = connectionConfig["zoneA"].get<int>();
        i32 zoneB = connectionConfig["zoneB"].get<int>();
        zonesI[zoneA]->addConnection(connectionConfig);
        zonesI[zoneB]->addConnection(connectionConfig);
    }
}
void TemplateInfo::printTemplate() {
    std::cerr << "Template name: " << getName() << "\n";
    std::cerr << "Template description: " << getDescription() << "\n";
    std::cerr << "Template map size: " << getMapSize() << "\n";
    std::cerr << "Template difficulty: " << getDifficulty() << "\n";

    for(auto zone : zonesI) {
        zone.second->printZone();
    }


}