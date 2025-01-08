#pragma once

#include "./global/Global.h"
#include <nlohmann/json.hpp> 

enum class Terrain;

class TownInfo;
class MineInfo;

using json = nlohmann::json;

class ConnectionInfo;
class ZoneInfo;


using ZoneI = std::shared_ptr<ZoneInfo>;
using ZonesI = std::map<i32, ZoneI>;


class TemplateInfo {
public:

    TemplateInfo();

    ZonesI & getZonesI();

    void setName(std::string name);
    void setDescription(std::string description);
    void setMapSize(std::string mapSize);
    void setDifficulty(std::string difficulty);

    std::string getName();
    std::string getDescription();
    std::string getMapSize();
    std::string getDifficulty();

    void deserialize(const json& config);
    void printTemplate();

private:
    std::string name;
    std::string description;
    std::string mapSize;
    std::string difficulty; 
    ZonesI zonesI;
};