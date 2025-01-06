#pragma once

#include "./global/Global.h"
#include <nlohmann/json.hpp> 

enum class Terrain;

class Town;

using json = nlohmann::json;

class ZoneConnection {

public:
    ZoneConnection();
    i32 getZoneA();
    i32 getZoneB();
    i32 getOtherZone(i32 zone);
    void setZoneA(i32 zone);
    void setZoneB(i32 zone);
    

private:
    i32 zoneA;
    i32 zoneB;

};

class ZoneInfo {
public:
    ZoneInfo(bool debug);
    void addConnection(const json& connectionConfig);
    std::vector<ZoneConnection> getConnections();

    void addTown(Town town);
    std::vector<Town> getTowns();

    void setId(i32 id);
    void setSize(std::string size);
    void setOwner(i32 ownerId);
    void setHero(std::string hero);
    void setTerrain(Terrain terrain);
    i32 getId();
    std::string getSize();
    i32 getOwner();
    std::string getHero();
    Terrain getTerrain();
    void deserializeZone(const json& config);
    void printZone();
private:
    bool debug;
    i32 id;
    i32 ownerId;
    std::string size;
    std::string hero;
    Terrain terrain;
    std::vector<Town> towns;
    std::vector<ZoneConnection> connections;


};


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