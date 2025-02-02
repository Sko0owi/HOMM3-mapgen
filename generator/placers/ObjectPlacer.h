#pragma once

#include <bits/stdc++.h>

class Object;
class Map;
class TemplateInfo;
class RNG;
class int3;
class TreasuresInfo;
class MineInfo;
class Tile;
class Zone;

class ObjectPlacer {
public:

    ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng);

    void placeObjects();

    void placeTowns();

    void placeMines();

    void placeTreasures();

    void placeBlockOfTreasures(std::shared_ptr<Zone> zonePtr, TreasuresInfo treasuresInfo);

    void placeTreasureBuilding(std::shared_ptr<Zone> zonePtr, TreasuresInfo treasuresInfo);

    void preparePossibleTreasures();

    void preparePossibleBlockSizes();

    bool placeMine(MineInfo mineI, std::shared_ptr<Object> centerPtr, std::shared_ptr<Zone> zonePtr, bool firstBasicMine = false, bool deterministic = false);

    void recalculateDistances();

    void calculateShortestDistances(std::shared_ptr<Object>& object);

    bool canPlaceObject(int3 pos, int3 size, bool extended=false);

    std::vector<std::vector<int>> getObjectsMap();
    void setObjectsMap(std::vector<std::vector<int>> objectsMap);

private:

    int mapWidth;
    int mapHeight;

    bool debug = false;

    Map & map;
    TemplateInfo & temp;
    RNG *rng;

    std::vector<std::vector<int>> objectsMap;
    
    std::vector<int3> possibleBlockSizes;

    std::map<std::shared_ptr<Zone>, std::vector<std::shared_ptr<Object>>> objects;

    std::map<std::shared_ptr<Zone>, std::vector<std::pair<std::pair<int,int>,std::shared_ptr<Tile>>>> zoneTiles;


    struct ObjectPtrCompare {
        bool operator()(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const {
            return a.get() < b.get();
        }
    };

    std::map<std::shared_ptr<Object>, std::vector<std::vector<int>>, ObjectPtrCompare> objectsDistances;


};