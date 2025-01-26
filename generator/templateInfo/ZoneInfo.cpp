#include "./ZoneInfo.h"
#include "./TemplateInfo.h"
#include "./ConnectionInfo.h"
#include "./TownInfo.h"
#include "./MineInfo.h"
#include "./TreasuresInfo.h"
#include "../gameInfo/Terrain.h"

ZoneInfo::ZoneInfo(bool debug)
    : debug(debug), id(0), size(0), maxMinesCount(0) {
}

void ZoneInfo::addConnection(const json& connectionConfig) {
    ConnectionInfo zoneConnection;

    
    int zoneA = TemplateInfo::getOrError<int>(connectionConfig, "zoneA");
    int zoneB = TemplateInfo::getOrError<int>(connectionConfig, "zoneB");
    int tier = connectionConfig.value("tier", 3);
    std::string type = connectionConfig.value("type", "narrow");

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

std::vector<TownInfo>& ZoneInfo::getTowns() {
    return towns;
}

std::vector<MineInfo>& ZoneInfo::getMines() {
    return mines;
}

void ZoneInfo::setId(i32 id) {
    this->id = id;
}

void ZoneInfo::setSize(i32 size) {
    this->size = size;
}

void ZoneInfo::setTerrain(Terrain terrain) {
    this->terrain = terrain;
}
void ZoneInfo::setMaxMinesCount(i32 maxMinesCount) {
    this->maxMinesCount = maxMinesCount;
}

void ZoneInfo::setTreasuresInfo(TreasuresInfo treasureInfo) {
    this->treasureInfo = treasureInfo;
}

void ZoneInfo::setDifficulty(std::string difficulty){
    this->difficulty = difficulty;
}

TreasuresInfo& ZoneInfo::getTreasuresInfo() {
    return treasureInfo;
}


i32 ZoneInfo::getId() {
    return id;
}

i32 ZoneInfo::getSize() {
    return size;
}

std::string ZoneInfo::getDifficulty() {
    return difficulty;
}

Terrain ZoneInfo::getTerrain() {
    return terrain;
}
i32 ZoneInfo::getMaxMinesCount() {
    return maxMinesCount;
}


int decodeOwner(std::string owner) {
    if(owner == "None") return 0;

    return std::stoi(owner.substr(owner.find_first_of("0123456789")));

}

int decodeSize(std::string size) {
    if(size == "S") return 1000;
    if(size == "M") return 2000;
    if(size == "L") return 3000;
    return 0;
}


ZoneRichness decodeRichness(std::string richness) {
    if (richness == "Low")
        return ZoneRichness::Low;
    if (richness == "Normal")
        return ZoneRichness::Normal;
    if (richness == "High")
        return ZoneRichness::Rich;
    return ZoneRichness::Normal;
}

void ZoneInfo::deserializeZone(const json& config) {
    
    i32 id = TemplateInfo::getOrError<int>(config, "id");
    std::string size = TemplateInfo::getOrError<std::string>(config, "size");
    std::string difficulty = TemplateInfo::getOrError<std::string>(config, "difficulty");

    
    std::string terrain = config.value("terrain", "Grass");

    std::string zoneRichness = config.value("richness", "Normal");
    TreasuresInfo treasuresInfo(decodeRichness(zoneRichness));

    i32 town_count = config.value("numberOfTowns", 0);
    for (int i = 0; i < town_count; i++) {

        std::string townFaction = TemplateInfo::getOrError<std::string>(config["towns"][i], "faction");
        std::string townOwner = TemplateInfo::getOrError<std::string>(config["towns"][i], "owner");
        int townMinCount = config["towns"][i].value("minCount", 1);
        

        TownInfo townI(stringToFaction(townFaction), decodeOwner(townOwner), townMinCount);

        addTown(townI);
    }

    i32 maxMinesCount = config.value("maxNumberOfMines", 0);
    i32 mine_count = config.value("numberOfMineTypes", 0);
    for(int i = 0; i < mine_count; i++) {
    
        std::string mineType = TemplateInfo::getOrError<std::string>(config["mines"][i], "type");
        std::string mineOwner = config["mines"][i].value("owner", "None");
        int mineMinCount = config["mines"][i].value("minCount", 1);

        MineInfo mineI(stringToMineType(mineType), decodeOwner(mineOwner), mineMinCount);
        
        addMine(mineI);
    }

    setTreasuresInfo(treasuresInfo);
    setId(id);
    setSize(decodeSize(size));
    setTerrain(stringToTerrain(terrain));
    setMaxMinesCount(maxMinesCount);
    setDifficulty(difficulty);
}

void ZoneInfo::printZone() {
    std::cerr << "Zone id: " << id << "\n";
    std::cerr << "Zone size: " << size << "\n";
    for(auto connection : connections) {
        std::cerr << "Connection: \n" << \
        "ZoneA: " << connection.getZoneA() << " " << \
        "ZoneB: " << connection.getZoneB() << " " << \
        "Tier: " << connection.getTier() << "\n";

    }
}