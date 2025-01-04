#include "lua_helpers.hpp"
#include <sstream>

#include "../game_info/Tile.h"
#include "../Template.h"
#include "./Map.h"
#include "./Zone.h"
#include "./game_info/Terrain.h"
#include "./game_info/Town.h"

using json = nlohmann::json;

// @function    AddPlayer
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      integer         playerId    player ID.
void AddPlayer(std::ofstream& luaFile, int playerId) {
    luaFile << "instance:player(homm3lua.PLAYER_" << playerId << ")\n";
}

// @function    AddTown
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      json        zone        zone description from json, like player id, town xyz position.
// @tparam      boolean     is_main     tells if is main town.
void AddTown(std::ofstream &luaFile, std::shared_ptr<Zone>& zone, Town town, bool is_main){
    std::string faction = "TOWN_" + factionToString(town.getFaction());
    i32 Id = zone->getOwnerId();
    i32 X = zone->getPosition().x;
    i32 Y = zone->getPosition().y;

    luaFile << "instance:town(homm3lua." << faction
            << ", {x=" << X
            << ", y=" << Y
            << ", z=0}, homm3lua.PLAYER_" << Id
            << ", " << is_main << ")\n";
}

// @function    AddHero
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      json        zone        zone description from json, like player id, hero xyz position.
void AddHero(std::ofstream& luaFile, std::shared_ptr<Zone>& zone) {
    std::string hero = "HERO_CHRISTIAN";
    i32 Id = zone->getOwnerId();
    i32 X = zone->getPosition().x;
    i32 Y = zone->getPosition().y;
    luaFile << "instance:hero(homm3lua." << hero
            << ", {x=" << X
            << ", y=" << Y + 1
            << ", z=0}, homm3lua.PLAYER_" << Id
            << ")\n\n";
}

// @function    AddTerrainTiles
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      Map         map         object of map class with finised setup.
void AddTerrainTiles(std::ofstream& luaFile, Map& map){
    luaFile << "instance:terrain(function (x, y, z)\n";

    for(int x = 0; x < map.getWidth(); x++){
        for(int y = 0; y < map.getHeight(); y++){
            auto tile = map.getTile(x, y);
            int tileZoneId = tile->getZoneId();
            bool isTileEdge = tile->getIsEdge();

            std::string terrain;

            if(!isTileEdge)
            {
                terrain = terrainToString(map.getZones()[tileZoneId]->getTerrain());
                luaFile << "if x == " << x << " and y == " << y << " then return homm3lua.TERRAIN_" << terrain << " end\n";
            }
        }
    }

    luaFile << "end)\n";
}

// @function    AddEdgeObstacles
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      Map         map         object of map class with finised setup.
void AddEdgeObstacles(std::ofstream& luaFile, Map& map){
    for(int x = 0; x < map.getWidth(); x++){
        for(int y = 0; y < map.getHeight(); y++){
            auto tile = map.getTile(x, y);
            bool isTileEdge = tile->getIsEdge();

            std::string terrain;
            
            if (isTileEdge) {
                AddObstacle(luaFile, "Pine Trees", x, y, 0);
            }
            //Example Obstacles
            // Pine Trees
            // Rock
            // Mushrooms
            // Mountain
        }
    }
}

// @function    AddTerrain
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      string      terrain     type of terrain, GRASS by default, See TERRAIN_*.
void AddTerrain(std::ofstream& luaFile, std::string terrain){
    luaFile << "instance:terrain(homm3lua.TERRAIN_" << terrain <<  ")\n";
}

// @function    AddHeader
// @tparam      ofstream    luaFile     file where we save lua script parts. 
void AddHeader(std::ofstream& luaFile){
        luaFile << R"(
package.cpath = package.cpath .. ';dist/?.so;../dist/?.so'
local homm3lua = require('homm3lua'))";

}

// @function    AddCreature
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      crerature        type of creature, See CREATURE_*.
// @tparam      integer     x                position on x axis of creature.
// @tparam      integer     y                position on y axis of creature.
// @tparam      integer     z                position on z axis of creature.
// @tparam      integer     quantity         quantity of creatures.
// @tparam      string      disposition      disposition of creature, See DISPOSITION_*.
// @tparam      boolean     never_flees      disallows creatures to flee.
// @tparam      boolean     does_not_grow    disallows creatures to grow.
void AddCreature(std::ofstream& luaFile, std::string creature, int x, int y, int z, int quantity, std::string disposition, bool never_flees, bool does_not_grow){
    luaFile << "instance:creature(homm3lua.CREATURE_" << creature << ", {x=" << x << ", y=" << y << ", z=" << z << "}, " << quantity << ", homm3lua.DISPOSITION_" << disposition << ", " << never_flees << ", " << does_not_grow <<  ")\n";  
}

// @function    AddMine
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      mine             type of mine, See MINE_*.
// @tparam      integer     x                position on x axis of mine.
// @tparam      integer     y                position on y axis of mine.
// @tparam      integer     z                position on z axis of mine.
// @tparam      integer     owner_id         owner of mine, by default -1, which means neutral.

void AddMine(std::ofstream& luaFile, std::string mine, int x, int y, int z, int owner_id){
    std::string owner = owner_id < 0 ? "OWNER_NEUTRAL" : "PLAYER_" + std::to_string(owner_id);
    luaFile << "instance:mine(homm3lua.MINE_" << mine << ", {x=" << x << ", y=" << y << ", z=" << z << "}, homm3lua." << owner << ")\n";
}

// @function    AddResource
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      mine             type of mine, See RESOURCE_*.
// @tparam      integer     x                position on x axis of resource.
// @tparam      integer     y                position on y axis of resource.
// @tparam      integer     z                position on z axis of resource.
// @tparam      integer     quantity         quantity of resource.
void AddResource(std::ofstream& luaFile, std::string resource, int x, int y, int z, int quantity){
    luaFile << "instance:resource(homm3lua.RESOURCE_" << resource << ", {x=" << x << ", y=" << y << ", z=" << z << "}, " << quantity << ")\n";  
}

// @function    AddArtifact
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      artifact         type of mine, See ARTIFACT_*.
// @tparam      integer     x                position on x axis of artifact.
// @tparam      integer     y                position on y axis of artifact.
// @tparam      integer     z                position on z axis of artifact.
void AddArtifact(std::ofstream& luaFile, std::string artifact, int x, int y, int z){
    luaFile << "instance:artifact(homm3lua.ARTIFACT_" << artifact << ", {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
}

// @function    AddObstacle
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      obstacle         type of obstacle.
// @tparam      integer     x                position on x axis of obstacle.
// @tparam      integer     y                position on y axis of obstacle.
// @tparam      integer     z                position on z axis of obstacle.
void AddObstacle(std::ofstream& luaFile, std::string obstacle, int x, int y, int z){
    luaFile << "instance:obstacle('" << obstacle << "', {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
}

// @function    AddSign
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      text             text on sign.
// @tparam      integer     x                position on x axis of sign.
// @tparam      integer     y                position on y axis of sign.
// @tparam      integer     z                position on z axis of sign.
void AddSign(std::ofstream& luaFile, std::string text, int x, int y, int z){
    luaFile << "instance:sign('" << text << "', {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
}