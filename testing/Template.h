
#include "./global/Global.h"
#include <nlohmann/json.hpp> 

using json = nlohmann::json;

class ZoneConnection {

public:
    ZoneConnection();
    i32 getZoneA();
    i32 getZoneB();
    void setZoneA(i32 zone);
    void setZoneB(i32 zone);

private:
    i32 zoneA;
    i32 zoneB;

};

class ZoneInfo {
public:
    ZoneInfo();
    void addConnection(const json& connectionConfig);
    std::vector<ZoneConnection> getConnections();
    void setId(i32 id);
    void setSize(std::string size);
    void setPlayer(std::string player);
    void setHero(std::string hero);
    i32 getId();
    std::string getSize();
    std::string getPlayer();
    std::string getHero();
    void deserializeZone(const json& config);
    void printZone();
private:

    i32 id;
    std::string size;
    std::string player;
    std::string hero;
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