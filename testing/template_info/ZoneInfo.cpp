#include "./ZoneInfo.h"
#include "./ConnectionInfo.h"
#include "./TownInfo.h"
#include "./MineInfo.h"
#include "../game_info/Terrain.h"

ZoneInfo::ZoneInfo(bool debug) {
    id = 0;
    size = 0;
    hero = "";
    debug = debug;
}

void ZoneInfo::addConnection(const json& connectionConfig) {
    ConnectionInfo zoneConnection;

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

std::vector<ConnectionInfo> ZoneInfo::getConnections() {
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

void ZoneInfo::setSize(i32 size) {
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

i32 ZoneInfo::getSize() {
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

int decodeSize(std::string size) {
    if(size == "SMALL") return 1000;
    if(size == "MEDIUM") return 2000;
    if(size == "LARGE") return 3000;
    return 0;
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
    setSize(decodeSize(size));
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