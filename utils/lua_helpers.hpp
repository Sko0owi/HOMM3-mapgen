#ifndef LUA_HELPERS_HPP
#define LUA_HELPERS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp> 
#include </usr/include/lua.hpp>

class Map;
class Zone;
class Town;

void AddPlayer(std::ofstream& luaFile, int playerId);
void AddTown(std::ofstream &luaFile, std::shared_ptr<Zone> &zone, Town town, bool is_main = true);
void AddHero(std::ofstream &luaFile, std::shared_ptr<Zone> &zone);
void AddTerrainTiles(std::ofstream& luaFile, Map& map);
void AddTerrain(std::ofstream& luaFile, std::string terrain = "GRASS");
void AddHeader(std::ofstream& luaFile);
void AddCreature(std::ofstream& luaFile, std::string creature, int x, int y, int z, int quantity, std::string disposition, bool never_flees, bool does_not_grow);
void AddMine(std::ofstream& luaFile, std::string mine, int x, int y, int z, int owner_id = -1);
void AddResource(std::ofstream& luaFile, std::string resource, int x, int y, int z, int quantity);
void AddArtifact(std::ofstream& luaFile, std::string artifact, int x, int y, int z);
void AddObstacle(std::ofstream& luaFile, std::string obstacle, int x, int y, int z);
void AddSign(std::ofstream& luaFile, std::string text, int x, int y, int z);

#endif