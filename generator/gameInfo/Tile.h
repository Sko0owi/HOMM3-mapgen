#pragma once

#include <bits/stdc++.h>

class Tile {
public: 


    Tile();
    void setZoneId(int zoneId);
    int getZoneId();

    void setIsBorder(bool isBorder);
    bool getIsBorder();

    void setIsGate(bool isGate);
    bool getIsGate();

    void setIsGuard(bool isGuard);
    bool getIsGuard();

    void setNeighbourZones(int zoneId);
    std::set<int> getNeighbourZones();

    void setIsRoad(bool isRoad);
    bool getIsRoad();

    void setIsExtension(bool isExtension);
    bool getIsExtension();

    void setTier(int tier);
    int getTier();

private:
    int zoneId;
    int tier;
    
    bool isBorder;
    bool isGate;
    bool isGuard;
    bool isRoad;
    bool isExtension;

    std::set<int> neighbourZones;
};