#ifndef LUA_HELPERS_HPP
#define LUA_HELPERS_HPP

#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp> 
#include </usr/include/lua/lua.hpp>
#include "../game_info/Object.h"

using MapObjects = std::vector<Object>;

class Map;
class Zone;
class Town;
class Treasure;
class Mine;
class ObjectPlacer;
class RNG;
class BorderPlacer;

void AddPlayer(std::ofstream& luaFile, int playerId);
void AddTown(std::ofstream &luaFile, Town town, bool is_main = true);
void AddHero(std::ofstream &luaFile, std::shared_ptr<Zone> &zone);
void AddTerrainTiles(std::ofstream& luaFile, Map& map);
void AddBorderObstacles(std::ofstream &luaFile, Map &map);
void AddTerrain(std::ofstream &luaFile, std::string terrain = "GRASS");
void AddHeader(std::ofstream& luaFile);
void AddCreature(std::ofstream& luaFile, std::string creature, int x, int y, int z, int quantity, std::string disposition, bool never_flees, bool does_not_grow);
void AddMine(std::ofstream& luaFile, Mine mine);
void AddResource(std::ofstream& luaFile, Treasure treasure);
void AddArtifact(std::ofstream& luaFile, Treasure treasure);
void AddObstacle(std::ofstream& luaFile, std::string obstacle, int x, int y, int z);
void AddSign(std::ofstream& luaFile, std::string text, int x, int y, int z);
void AddRoads(std::ofstream &luaFile, Map &map, std::shared_ptr<ObjectPlacer> objectPlacer, RNG *rng);
void AddMapObjects(std::ofstream &luaFile, Map& map, std::shared_ptr<ObjectPlacer> objectPlacer, RNG *rng);
std::vector<std::pair<int, int>> getValidTiles(int zoneId, Map &map, std::shared_ptr<ObjectPlacer> objectPlacer, Object object);

#endif