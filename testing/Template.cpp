#include "./Template.h"
#include "../game_info/Terrain.h"
#include "../game_info/Town.h"

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

i32 ZoneConnection::getOtherZone(i32 zone) {
    if (zone == zoneA) {
        return zoneB;
    }
    return zoneA;
}

void ZoneConnection::setZoneA(i32 zone) {
    zoneA = zone;
}

void ZoneConnection::setZoneB(i32 zone) {
    zoneB = zone;
}

ZoneInfo::ZoneInfo(bool debug) {
    id = 0;
    size = "";
    ownerId = 0;
    hero = "";
    debug = debug;
}

void ZoneInfo::addConnection(const json& connectionConfig) {
    ZoneConnection zoneConnection;
    std::cerr << "ADDING " << connectionConfig["zoneA"].get<int>() << " " << connectionConfig["zoneB"].get<int>() << "\n";
    int zoneA = connectionConfig["zoneA"].get<int>();
    int zoneB = connectionConfig["zoneB"].get<int>();
    zoneConnection.setZoneA(zoneA);
    zoneConnection.setZoneB(zoneB);

    std::cerr << "ADD2 " << zoneA << " " << zoneB << "\n";
    connections.push_back(zoneConnection);

    std::cerr << "CURR STATE\n";
    for (auto c : connections){
        std::cerr << c.getZoneA() << " " << c.getZoneB() << "\n";
    }
    std::cerr << "\n";
}

std::vector<ZoneConnection> ZoneInfo::getConnections() {
    std::cerr << "SIZE " << connections.size() <<  "\n";
    for(auto e : connections){
        std::cerr << e.getZoneA() << " " << e.getZoneB() << "\n";
    }
    return connections;
}

void ZoneInfo::addTown(Town town) {
    towns.push_back(town);
}

std::vector<Town> ZoneInfo::getTowns() {
    return towns;
}

void ZoneInfo::setId(i32 id) {
    this->id = id;
}

void ZoneInfo::setSize(std::string size) {
    this->size = size;
}

void ZoneInfo::setOwner(i32 ownerId) {
    this->ownerId = ownerId;
}

void ZoneInfo::setHero(std::string hero) {
    this->hero = hero;
}

void ZoneInfo::setTerrain(Terrain terrain) {
    this->terrain = terrain;
}

i32 ZoneInfo::getId() {
    return id;
}

std::string ZoneInfo::getSize() {
    return size;
}

i32 ZoneInfo::getOwner() {
    return ownerId;
}

std::string ZoneInfo::getHero() {
    return hero;
}

Terrain ZoneInfo::getTerrain() {
    return terrain;
}

int decodeOwner(std::string owner) {
    if(owner == "NONE") return 0;

    return std::stoi(owner.substr(owner.find_first_of("0123456789")));

}

void ZoneInfo::deserializeZone(const json& config) {

    i32 id = config["id"].get<int>();
    std::string size = config["size"].get<std::string>();
    std::string ownerStr = config["owner"].get<std::string>();
    std::string hero = config["hero"].get<std::string>();
    
    std::string terrain = config["terrain"].get<std::string>();

    i32 town_count = config["towns"]["count"].get<int>();
    for (int i = 0; i < town_count; i++) {
        Town town;
        std::string town_faction = config["towns"]["faction"].get<std::string>();
        if(debug)
            std::cerr << "Sanity check1: " << town_faction << "\n";
        town.setFaction(stringToFaction(config["towns"]["faction"].get<std::string>()));
        if(debug)
            std::cerr << "Sanity check: " << factionToString(town.getFaction()) << "\n";
        addTown(town);
    }

    setId(id);
    setSize(size);
    setOwner(decodeOwner(ownerStr));
    setHero(hero);
    setTerrain(stringToTerrain(terrain));
    
}

void ZoneInfo::printZone() {
    std::cerr << "Zone id: " << id << "\n";
    std::cerr << "Zone size: " << size << "\n";
    std::cerr << "Zone owner: " << ownerId << "\n";
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
        auto zone = std::make_shared<ZoneInfo>(config["debug"]);
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