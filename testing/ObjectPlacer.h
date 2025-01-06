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

    


private:

    int mapWidth;
    int mapHeight;

    Map & map;
    TemplateInfo & temp;
    RNG *rng;

};