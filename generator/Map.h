#pragma once


#include<bits/stdc++.h>
#include "./global/Global.h"
#include "./gameInfo/Object.h"

class Tile;
class Zone;
class TemplateInfo;
class RNG;
class Town;
class ObjectPlacer;
class GuardPlacer;
class Object;

using MapZones = std::map<i32, std::shared_ptr<Zone>>;
using MapObjects = std::vector<Object>;
using ConnectedPoints = std::vector<std::tuple<int, int, int, int, bool, int>>;

class Map
{
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

    void setConnectedPairs(ConnectedPoints connectedPairs);
    ConnectedPoints  getConnectedPairs();

    void addObject(Object object);
    void setMapObjects(MapObjects mapObjects);
    MapObjects  getMapObjects();

private:
    i32 width;
    i32 height;
    MapZones zones;
    

    std::map<i32, map<i32, std::shared_ptr<Tile>>> Tiles;
    ConnectedPoints connectedPairs;
    
    MapObjects mapObjects;

    RNG *rng;
};