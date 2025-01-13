#pragma once

#include "./global/Global.h"
#include "./TreasuresInfo.h"
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
    std::vector<TownInfo>& getTowns();
    std::vector<MineInfo>& getMines();

    void setTreasuresInfo(TreasuresInfo treasureInfo);
    TreasuresInfo& getTreasuresInfo();

    void setId(i32 id);
    void setSize(i32 size);
    void setTerrain(Terrain terrain);
    void setDifficulty(std::string difficulty);
    void setMaxMinesCount(i32 maxMinesCount);

    i32 getId();
    i32 getSize();
    std::string getDifficulty();
    Terrain getTerrain();
    i32 getMaxMinesCount();
    
    void deserializeZone(const json& config);
    void printZone();
private:
    bool debug;
    i32 id;
    i32 size;
    std::string difficulty;

    Terrain terrain;

    i32 maxMinesCount;

    TreasuresInfo treasureInfo;

    std::vector<TownInfo> towns;
    std::vector<MineInfo> mines;
    std::vector<ConnectionInfo> connections;


};
