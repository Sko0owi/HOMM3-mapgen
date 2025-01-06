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

    void setNeighbourZones(int zoneId);
    std::set<int> getNeighbourZones();

    void setIsRoad(bool isRoad);
    bool getIsRoad();

    void setIsExtension(bool isExtension);
    bool getIsExtension();

private:
    int zoneId;
    bool isBorder;
    bool isGate;
    bool isRoad;
    bool isExtension;

    std::set<int> neighbourZones;
};