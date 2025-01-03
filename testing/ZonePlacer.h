
#include <bits/stdc++.h>


class Zone;
class Map;
class TemplateInfo;
class int3;
class float3;
class RNG;

class ZonePlacer {
public: 
    ZonePlacer(Map & map, TemplateInfo & temp, RNG *rng);

    int3 getRealCoords(float3 f);
    void calculatePaths();
    void placeOnGrid();
    void placeZones();

    void generateZones();

    void paintTiles();
    
    bool areConnected(int ZoneA, int ZoneB);
    void determineZoneEdges();
    void modifyRandomConnectionTile(int range=1);

private:
    int mapWidth;
    int mapHeight;
    bool debug = false;

    Map & map;
    TemplateInfo & temp;
    
    std::map<int, std::map<int, size_t>> DistancesBetweenZones;

    RNG *rng;

    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> connections;
    std::pair<int, int> getRandomConnectionPoint(int zoneA, int zoneB);
    void addConnectionPoint(int zoneA, int zoneB, int x, int y);
};