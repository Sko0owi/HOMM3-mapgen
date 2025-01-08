#pragma once

#include <bits/stdc++.h>

class Object;
class Map;
class TemplateInfo;
class RNG;
class int3;

class ObjectPlacer {
public:

    ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng);

    void placeObjects();

    void placeTowns();

    void placeMines();

    void recalculateDistances();

    void calculateShortestDistances(std::shared_ptr<Object> object);

    bool canPlaceObject(int3 pos, int3 size);

private:

    int mapWidth;
    int mapHeight;

    Map & map;
    TemplateInfo & temp;
    RNG *rng;

    std::vector<std::vector<int>> objectsMap;

    std::vector<std::shared_ptr<Object>> objects;


    struct ObjectPtrCompare {
        bool operator()(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) const {
            return a.get() < b.get();
        }
    };

    std::map<std::shared_ptr<Object>, std::vector<std::vector<int>>, ObjectPtrCompare> objectsDistances;


};