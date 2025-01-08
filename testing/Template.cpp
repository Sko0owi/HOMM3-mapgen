#include "./Template.h"
#include "../game_info/Terrain.h"
#include "../template_info/MineInfo.h"
#include "../template_info/TownInfo.h"

ZoneConnection::ZoneConnection() {
    zoneA = 0;
    zoneB = 0;
    tier = 0;
    type = "narrow";
}

i32 ZoneConnection::getZoneA() {
    return zoneA;
}

i32 ZoneConnection::getZoneB() {
    return zoneB;
}

i32 ZoneConnection::getTier() {
    return tier;
}

string ZoneConnection::getType() {
    return type;
}

i32 ZoneConnection::getOtherZone(i32 zone) {
    if (zone == zoneA)
    {
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

void ZoneConnection::setTier(i32 tier) {
    this->tier = tier;
}

void ZoneConnection::setType(string type) {
    this->type = type;
}

ZoneInfo::ZoneInfo(bool debug) {
    id = 0;
    size = "";
    hero = "";
    debug = debug;
}

void ZoneInfo::addConnection(const json& connectionConfig) {
    ZoneConnection zoneConnection;

    int zoneA = connectionConfig["zoneA"].get<int>();
    int zoneB = connectionConfig["zoneB"].get<int>();
    int tier = connectionConfig["tier"].get<int>();
    string type = connectionConfig["type"].get<string>();

    zoneConnection.setZoneA(zoneA);
    zoneConnection.setZoneB(zoneB);
    zoneConnection.setTier(tier);
    zoneConnection.setType(type);


    connections.push_back(zoneConnection);
}

std::vector<ZoneConnection> ZoneInfo::getConnections() {
    return connections;
}

void ZoneInfo::addTown(TownInfo town) {
    towns.push_back(town);
}

void ZoneInfo::addMine(MineInfo mine) {
    mines.push_back(mine);
}

std::vector<TownInfo> ZoneInfo::getTowns() {
    return towns;
}

std::vector<MineInfo> ZoneInfo::getMines() {
    return mines;
}

void ZoneInfo::setId(i32 id) {
    this->id = id;
}

void ZoneInfo::setSize(std::string size) {
    this->size = size;
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
    std::string hero = config["hero"].get<std::string>();
    
    std::string terrain = config["terrain"].get<std::string>();

    i32 town_count = config["number_of_towns"].get<int>();
    for (int i = 0; i < town_count; i++) {

        std::string townFaction = config["towns"][i]["faction"].get<std::string>();
        std::string townOwner = config["towns"][i]["owner"].get<std::string>();
        int townDensity = config["towns"][i]["density"].get<int>();
        int townMinCount = config["towns"][i]["min_count"].get<int>();

        TownInfo townI(stringToFaction(townFaction), decodeOwner(townOwner), townDensity, townMinCount);

        addTown(townI);
    }

    i32 mine_count = config["number_of_mines"].get<int>();
    for(int i = 0; i < mine_count; i++) {
    
        std::string mineType = config["mines"][i]["type"].get<std::string>();
        std::string mineOwner = config["mines"][i]["owner"].get<std::string>();
        int mineDensity = config["mines"][i]["density"].get<int>();
        int mineMinCount = config["mines"][i]["min_count"].get<int>();

        MineInfo mineI(stringToMineType(mineType), decodeOwner(mineOwner), mineDensity, mineMinCount);
        
        addMine(mineI);
    }

    setId(id);
    setSize(size);
    setHero(hero);
    setTerrain(stringToTerrain(terrain));
    
}

void ZoneInfo::printZone() {
    std::cerr << "Zone id: " << id << "\n";
    std::cerr << "Zone size: " << size << "\n";
    std::cerr << "Zone hero: " << hero << "\n";
    for(auto connection : connections) {
        std::cerr << "Connection: \n" << \
        "ZoneA: " << connection.getZoneA() << " " << \
        "ZoneB: " << connection.getZoneB() << " " << \
        "Tier: " << connection.getTier() << "\n";

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