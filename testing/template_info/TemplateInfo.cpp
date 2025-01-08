#include "../game_info/Terrain.h"
#include "./TemplateInfo.h"
#include "./MineInfo.h"
#include "./TownInfo.h"
#include "./ConnectionInfo.h"
#include "./ZoneInfo.h"

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

    std::string name = getOrError<std::string>(config, "name");
    std::string difficulty = getOrError<std::string>(config, "difficulty");
    std::string description = getOrError<std::string>(config, "description");
    std::string mapSize = config.value("size", "MEDIUM");


    setName(name);
    setDescription(description);
    setMapSize(mapSize);
    setDifficulty(difficulty);
    
    const auto& zonesConfig = getOrError<json>(config, "zones");
    for (const auto &zoneConfig : zonesConfig)
    {   
        auto zone = std::make_shared<ZoneInfo>(config.value("debug", false));
        zone->deserializeZone(zoneConfig);
        zonesI[zone->getId()] = zone;
    }

    const auto& connectionsConfig = getOrError<json>(config, "connections");
    for (const auto& connectionConfig : connectionsConfig) {
        i32 zoneA =  getOrError<int>(connectionConfig, "zoneA");
        i32 zoneB =  getOrError<int>(connectionConfig, "zoneB");
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