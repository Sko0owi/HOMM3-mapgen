
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
    void addConnection(json& connectionConfig);
    std::vector<ZoneConnection> getConnections();
    void setId(i32 id);
    void setSize(i32 size);
    void setPlayer(i32 player);
    void setHero(i32 hero);
    i32 getId();
    i32 getSize();
    i32 getPlayer();
    i32 getHero();
    void deserializeZone(const json& config);
private:

    i32 id;
    i32 size;
    i32 player;
    i32 hero;
    std::vector<ZoneConnection> connections;


};

class TemplateInfo {
public:
    using Zones = std::map<i32, std::shared_ptr<ZoneInfo>>;

    Zones & getZones();

    void deserialize(json& config);

private:
    Zones zones;
};