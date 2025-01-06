#pragma once

#include <bits/stdc++.h>

class Tile {
public: 


    Tile();
    void setZoneId(int zoneId);
    int getZoneId();

    void setIsEdge(bool isEdge);
    bool getIsEdge();

    void setIsGate(bool isGate);
    bool getIsGate();

    void setIsMiddleGate(bool isMiddleGate);
    bool getIsMiddleGate();

    void setNeighbourZones(int zoneId);
    std::set<int> getNeighbourZones();

private:
    int zoneId;
    bool isEdge;
    bool isGate;
    bool isMiddleGate;

    std::set<int> neighbourZones;
};