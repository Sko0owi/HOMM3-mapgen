#include <bits/stdc++.h>

class Tile {
public: 


    Tile();
    void setZoneId(int zoneId);
    int getZoneId();

    void setIsEdge(bool isEdge);
    bool getIsEdge();

    void setIsGate(bool IsGate);
    bool getIsGate();

    void setNeighbourZones(int zoneId);
    int getNeighbourZones();

private:
    int zoneId;
    bool isEdge;
    bool isGate;

    std::set<int> neighbourZones;
};