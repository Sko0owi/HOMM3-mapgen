#pragma once


#include<bits/stdc++.h>
#include "../global/Global.h"

class Tile;
class Zone;
class TemplateInfo;
class RNG;

using MapZones = std::map<i32, std::shared_ptr<Zone>>;
class Map {
public:
    Map(RNG *rng);
    Map(i32 width, i32 height);
    ~Map();
    void generateMap(TemplateInfo &temp);
    void print();
    MapZones & getZones();
    i32 getWidth();
    i32 getHeight();

    bool isMiddle(i32 x, i32 y);

    std::shared_ptr<Tile> getTile(i32 x, i32 y);

    void setConnectedPairs(vector<std::tuple<int, int, int, int>> connectedPairs);
    vector<std::tuple<int, int, int, int>>  getConnectedPairs();

private:
    i32 width;
    i32 height;
    MapZones zones;

    std::map<i32, map<i32, std::shared_ptr<Tile>>> Tiles;
    vector<std::tuple<int, int, int, int>> connectedPairs;

    RNG *rng;
};