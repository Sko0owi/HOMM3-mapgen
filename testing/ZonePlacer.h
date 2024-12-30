
#include <bits/stdc++.h>
#include "./types/int3.h"


class Zone;


class ZonePlacer {
public: 
    void placeOnGrid();
    void placeZones();

private:
    std::map<i32, std::shared_ptr<Zone>> zones;
    int gridWidth;
    int gridHeight;
};