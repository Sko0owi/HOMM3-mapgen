#pragma once

#include "./global/Global.h"

class ConnectionInfo {

public:
    ConnectionInfo();
    i32 getZoneA();
    i32 getZoneB();
    i32 getTier();
    string getType();

    i32 getOtherZone(i32 zone);
    void setZoneA(i32 zone);
    void setZoneB(i32 zone);
    void setTier(i32 tier);
    void setType(string type);

private:
    i32 zoneA;
    i32 zoneB;
    i32 tier;

    string type;
};
