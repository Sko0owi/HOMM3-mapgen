#pragma once

#include "./global/Global.h"
#include <nlohmann/json.hpp> 

enum class Terrain;

class TownInfo;
class MineInfo;
class ConnectionInfo;

using json = nlohmann::json;

class ZoneInfo {
public:
    ZoneInfo(bool debug);
    void addConnection(const json& connectionConfig);
    std::vector<ConnectionInfo> getConnections();

    void addTown(TownInfo town);
    void addMine(MineInfo mine);
    std::vector<TownInfo> getTowns();
    std::vector<MineInfo> getMines();

    void setId(i32 id);
    void setSize(std::string size);
    void setHero(std::string hero);
    void setTerrain(Terrain terrain);
    i32 getId();
    std::string getSize();
    std::string getHero();
    Terrain getTerrain();
    void deserializeZone(const json& config);
    void printZone();
private:
    bool debug;
    i32 id;
    std::string size;
    std::string hero;
    Terrain terrain;
    std::vector<TownInfo> towns;
    std::vector<MineInfo> mines;
    std::vector<ConnectionInfo> connections;


};
