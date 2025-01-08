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