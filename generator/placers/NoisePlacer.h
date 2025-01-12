#pragma once
#include <bits/stdc++.h>


class Map;
class RNG;
class ObjectPlacer;
class int3;

class NoisePlacer {
public:

    NoisePlacer(Map &map, RNG *rng, std::shared_ptr<ObjectPlacer> objectPlacer);

    void placeNoise();

    void calculateDistances(int3 startPos);

    void fixMap(int3 startPos);
private:
    Map &map;
    RNG *rng;

    std::shared_ptr<ObjectPlacer> objectPlacer;

    std::map<int, std::map<int,int>> distances;
    std::map<int, std::map<int,int3>> from;

    std::vector<std::vector<int>> currentMap;
};