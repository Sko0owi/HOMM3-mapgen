
#include <bits/stdc++.h>

class Map;
class TemplateInfo;
class RNG;
class Object;
class Zone;
class ObjectPlacer;

using ConnectedPoints = std::vector<std::tuple<int, int, int, int, bool, int>>;
using MapObjects = std::vector<Object>;

class BorderPlacer {
public: 
    BorderPlacer(Map & map, TemplateInfo & temp, RNG *rng, std::shared_ptr<ObjectPlacer> objectPlacer);


    void generateBorders();

    bool areConnected(int ZoneA, int ZoneB);
    int getTier(int ZoneA, int ZoneB);
    std::string getType(int ZoneA, int ZoneB);
    bool isMapBorder(int x, int y);

    void findOuter(int X, int Y, int *outerX1, int *outerY1, int *outerX2, int *outerY2, int zone1Id, int zone2Id);
    bool isWide(int ZoneA, int ZoneB);
    void determineZoneBorders();
    void connectZones();

    void createExtenstion();
    void removeExtension();

    void fixBorders();

    ConnectedPoints getConnectedPairs();
    std::set<std::pair<int, int>> getMonolithConnections(); 
    void setWideConnections();

    void setMapObjects(MapObjects mapObjects);
    MapObjects  getMapObjects();

    std::vector<std::pair<int, int>> getZoneTiles(int zoneId);
    std::vector<std::pair<int, int>> getValidTiles(int zoneId, Object object);

    void placeMonolith(Object object);

private:
    int mapWidth;
    int mapHeight;
    int monolithCount = 0;
    bool debug = false;

    static constexpr int dx[] = {-1, 1, 0, 0};
    static constexpr int dy[] = {0, 0, -1, 1};

    Map &map;
    TemplateInfo &temp;
    RNG *rng;

    std::set<std::pair<int, int>> wideConnections;
    std::set<std::pair<int, int>> monolithConnections;
    std::shared_ptr<ObjectPlacer> objectPlacer;

    ConnectedPoints connectedPairs;
    MapObjects mapObjects;
};