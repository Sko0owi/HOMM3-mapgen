#ifndef LUA_HELPERS_HPP
#define LUA_HELPERS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp> 
#include </usr/include/lua5.4/lua.hpp>

void AddPlayer(std::ofstream& luaFile, int playerId);
void AddTown(std::ofstream& luaFile, const nlohmann::json& zone);
void AddHero(std::ofstream& luaFile, const nlohmann::json& zone);
void AddTerrain(std::ofstream& luaFile, std::string terrain = "GRASS");
void AddHeader(std::ofstream& luaFile);
void AddCreature(std::ofstream& luaFile, std::string creature, int x, int y, int z, int quantity, std::string disposition, bool b1, bool b2);
void AddMine(std::ofstream& luaFile, std::string mine, int x, int y, int z);
void AddResource(std::ofstream& luaFile, std::string resource, int x, int y, int z, int quantity);
void AddArtifact(std::ofstream& luaFile, std::string artifact, int x, int y, int z);
void AddObstacle(std::ofstream& luaFile, std::string obstacle, int x, int y, int z);
void AddSign(std::ofstream& luaFile, std::string text, int x, int y, int z);

#endif