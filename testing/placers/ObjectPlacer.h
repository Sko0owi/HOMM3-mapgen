#pragma once

#include <bits/stdc++.h>

class Object;
class Map;
class TemplateInfo;
class RNG;
class int3;
class MineInfo;
class Tile;
class Zone;

class ObjectPlacer {
public:

    ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng);

    void placeObjects();

    void placeTowns();

    void placeMines();

    bool placeMine(MineInfo mineI, std::shared_ptr<Object> centerPtr, std::shared_ptr<Zone> zonePtr, std::map<std::shared_ptr<Zone>, std::vector<std::pair<std::pair<int,int>,std::shared_ptr<Tile>>>> &zoneTiles, bool firstBasicMine = false, bool deterministic = false);

    void recalculateDistances();

    void calculateShortestDistances(std::shared_ptr<Object>& object);

    bool canPlaceObject(int3 pos, int3 size);

private:

    int mapWidth;
    int mapHeight;

    Map & map;
    TemplateInfo & temp;
    RNG *rng;

    std::vector<std::vector<int>> objectsMap;

    std::map<std::shared_ptr<Zone>, std::vector<std::shared_ptr<Object>>> objects;


    struct ObjectPtrCompare {
        bool operator()(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const {
            return a.get() < b.get();
        }
    };

    std::map<std::shared_ptr<Object>, std::vector<std::vector<int>>, ObjectPtrCompare> objectsDistances;


};