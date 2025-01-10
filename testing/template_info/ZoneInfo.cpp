#include "./ZoneInfo.h"
#include "./TemplateInfo.h"
#include "./ConnectionInfo.h"
#include "./TownInfo.h"
#include "./MineInfo.h"
#include "./TreasuresInfo.h"
#include "../game_info/Terrain.h"

ZoneInfo::ZoneInfo(bool debug)
    : debug(debug), id(0), size(0), hero(""), maxMinesCount(0) {
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


void ZoneInfo::setHero(std::string hero) {
    this->hero = hero;
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

TreasuresInfo& ZoneInfo::getTreasuresInfo() {
    return treasureInfo;
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
i32 ZoneInfo::getMaxMinesCount() {
    return maxMinesCount;
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
    std::string hero = TemplateInfo::getOrError<std::string>(config, "hero");
    
    std::string terrain = config.value("terrain", "GRASS");

    std::string zoneRichness = config.value("richness", "Normal");
    TreasuresInfo treasuresInfo(decodeRichness(zoneRichness));

    std::cerr << "Zone richness: " << zoneRichness << "\n";

    i32 town_count = TemplateInfo::getOrError<int>(config, "number_of_towns");
    for (int i = 0; i < town_count; i++) {

        std::string townFaction = TemplateInfo::getOrError<std::string>(config["towns"][i], "faction");
        std::string townOwner = TemplateInfo::getOrError<std::string>(config["towns"][i], "owner");
        int townDensity = TemplateInfo::getOrError<int>(config["towns"][i], "density");
        int townMinCount = TemplateInfo::getOrError<int>(config["towns"][i], "min_count");

        TownInfo townI(stringToFaction(townFaction), decodeOwner(townOwner), townDensity, townMinCount);

        addTown(townI);
    }

    i32 maxMinesCount = config.value("max_number_of_mines", 0);
    std::cerr << "Max mines count: " << maxMinesCount << "\n";
    i32 mine_count =  TemplateInfo::getOrError<int>(config, "number_of_mines");
    for(int i = 0; i < mine_count; i++) {
    
        std::string mineType = TemplateInfo::getOrError<std::string>(config["mines"][i], "type");
        std::string mineOwner = TemplateInfo::getOrError<std::string>(config["mines"][i], "owner");
        int mineMinCount = TemplateInfo::getOrError<int>(config["mines"][i], "min_count");

        MineInfo mineI(stringToMineType(mineType), decodeOwner(mineOwner), mineMinCount);
        
        addMine(mineI);
    }

    setTreasuresInfo(treasuresInfo);
    setId(id);
    setSize(decodeSize(size));
    setHero(hero);
    setTerrain(stringToTerrain(terrain));
    setMaxMinesCount(maxMinesCount);
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