
#include <bits/stdc++.h>


class Zone;
class Map;
class TemplateInfo;
class int3;
class float3;
class RNG;


typedef std::map<std::shared_ptr<Zone>, float3> TForceVector;
typedef std::map<std::shared_ptr<Zone>, float3> TBestSolution;
typedef std::map<std::shared_ptr<Zone>, float> TDistanceVector;


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
    bool isMapEdge(int x, int y);
    int countEdges(int x, int y);
    void determineZoneEdges();
    void modifyRandomConnectionTile(int range=1);

    void prepareZones();
    bool evaluateZones(TDistanceVector &distances, TDistanceVector &overlaps, TBestSolution &bestSolution);

    void attractConnectedZones(TForceVector &forces, TDistanceVector &distances);
    void separateOverlappingZones(TForceVector &forces, TDistanceVector &overlaps);
private:
    int mapWidth;
    int mapHeight;
    bool debug = false;

    Map & map;
    TemplateInfo & temp;
    
    std::map<int, std::map<int, size_t>> DistancesBetweenZones;

    float bestTotalDistance = 1000000000;
    float bestTotalOverlap = 1000000000;
    
    float gravityConstant = 1e-3f;
	float stiffnessConstant = 3e-3f;
	float stifness = 0.0f;
	float stiffnessIncreaseFactor = 1.03f;

    RNG *rng;

    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> connections;
    std::pair<int, int> getRandomConnectionPoint(int zoneA, int zoneB);
    void addConnectionPoint(int zoneA, int zoneB, int x, int y);
};