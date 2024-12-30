
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

private:
    int mapWidth;
    int mapHeight;

    Map & map;
    TemplateInfo & temp;

    std::map<int, std::map<int, size_t>> DistancesBetweenZones;

    RNG *rng;
};