#pragma once

#include <bits/stdc++.h>

class Object;
class Map;
class TemplateInfo;
class RNG;

class ObjectPlacer {
public:

    ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng);

    void placeObjects();

    void placeTowns();

    void placeMines();


private:

    int mapWidth;
    int mapHeight;

    Map & map;
    TemplateInfo & temp;
    RNG *rng;

    std::vector<std::vector<int>> objectsMap;

    std::vector<std::shared_ptr<Object>> objects;

    std::map<std::shared_ptr<Object>, std::vector<std::vector<int>>> objectsDistances;

};