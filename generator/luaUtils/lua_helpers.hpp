#ifndef LUA_HELPERS_HPP
#define LUA_HELPERS_HPP

#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp> 
#include </usr/include/lua/lua.hpp>
#include "../gameInfo/Object.h"

using MapObjects = std::vector<Object>;

class Map;
class Zone;
class Town;
class Treasure;
class Mine;
class GuardPlacer;
class RNG;
class TemplateInfo;
class Creature;

void AddPlayer(std::ofstream& luaFile, int playerId);
void AddTown(std::ofstream &luaFile, Town town, bool is_main = true);
void AddTerrain(std::ofstream &luaFile, std::string terrain = "GRASS");
void AddTerrainTiles(std::ofstream& luaFile, Map& map);
void AddBorderObstacles(std::ofstream &luaFile, Map &map);
void AddHeader(std::ofstream& luaFile);
void AddCreature(std::ofstream& luaFile, Creature creature);
void AddMine(std::ofstream& luaFile, Mine mine, Map &map);
void AddResource(std::ofstream& luaFile, Treasure treasure);
void AddArtifact(std::ofstream& luaFile, Treasure treasure);
void AddBuildingTreasure(std::ofstream& luaFile, Treasure treasure);
void AddObstacle(std::ofstream& luaFile, std::string obstacle, int x, int y, int z);
void AddSign(std::ofstream& luaFile, std::string text, int x, int y, int z);
void AddRoads(std::ofstream &luaFile, Map &map);
void AddMapObjects(std::ofstream &luaFile, Map& map);

#endif