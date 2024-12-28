#include "lua_helpers.hpp"

using json = nlohmann::json;

void AddPlayer(std::ofstream& luaFile, int playerId) {
    luaFile << "instance:player(homm3lua.PLAYER_" << playerId << ")\n";
}

void AddTown(std::ofstream& luaFile, const json& zone) {
    luaFile << "instance:town(homm3lua." << zone["player"].get<std::string>()
            << ", {x=" << (zone["id"].get<int>() * 5)
            << ", y=" << (zone["id"].get<int>() * 5)
            << ", z=0}, homm3lua.PLAYER_" << zone["id"]
            << ", true)\n";
}

void AddHero(std::ofstream& luaFile, const json& zone) {
    luaFile << "instance:hero(homm3lua." << zone["hero"].get<std::string>()
            << ", {x=" << (zone["id"].get<int>() * 5)
            << ", y=" << (zone["id"].get<int>() * 5 + 1)
            << ", z=0}, homm3lua.PLAYER_" << zone["id"]
            << ")\n\n";
}

void AddTerrain(std::ofstream& luaFile, std::string terrrain){
    luaFile << "instance:terrain(homm3lua.TERRAIN_" << terrrain << ")\n";
}

void AddHeader(std::ofstream& luaFile){
        luaFile << R"(
package.cpath = package.cpath .. ';dist/?.so;../dist/?.so'
local homm3lua = require('homm3lua'))";

}

void AddCreature(std::ofstream& luaFile, std::string creature, int x, int y, int z, int quantity, std::string disposition, bool b1, bool b2){
    luaFile << "instance:creature(homm3lua.CREATURE_" << creature << ", {x=" << x << ", y=" << y << ", z=" << z << "}, " << quantity << ",homm3lua.DISPOSITION_" << disposition << "," << b1 << " ," << b2 <<  ")\n";  
}

void AddMine(std::ofstream& luaFile, std::string mine, int x, int y, int z){
    luaFile << "instance:mine(homm3lua.MINE_" << mine << ", {x=" << x << ", y=" << y << ", z=" << z << "}, homm3lua.OWNER_NEUTRAL)\n";  

}

void AddResource(std::ofstream& luaFile, std::string resource, int x, int y, int z, int quantity){
    luaFile << "instance:resource(homm3lua.RESOURCE_" << resource << ", {x=" << x << ", y=" << y << ", z=" << z << "}, " << quantity << ")\n";  
}

void AddArtifact(std::ofstream& luaFile, std::string artifact, int x, int y, int z){
    luaFile << "instance:artifact(homm3lua.ARTIFACT_" << artifact << ", {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
}

void AddObstacle(std::ofstream& luaFile, std::string obstacle, int x, int y, int z){
    luaFile << "instance:obstacle('" << obstacle << "', {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
}

void AddSign(std::ofstream& luaFile, std::string text, int x, int y, int z){
    luaFile << "instance:sign('" << text << "', {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
}