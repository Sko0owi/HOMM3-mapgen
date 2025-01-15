#include "./lua_helpers.hpp"
#include <sstream>

#include "../gameInfo/Tile.h"
#include "../templateInfo/TemplateInfo.h"
#include "../templateInfo/ZoneInfo.h"
#include "../templateInfo/ConnectionInfo.h"
#include "./Map.h"
#include "./gameInfo/Zone.h"
#include "./gameInfo/Terrain.h"
#include "./gameInfo/Town.h"
#include "./gameInfo/Treasure.h"
#include "./gameInfo/Mine.h"
#include "./gameInfo/Creature.h"
#include "../global/Random.h"
#include "../placers/GuardPlacer.h"

using json = nlohmann::json;

// @function    AddPlayer
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      integer     playerId    player ID.
void AddPlayer(std::ofstream& luaFile, int playerId) {
    luaFile << "instance:player(homm3lua.PLAYER_" << playerId << ")\n";
}

// @function    AddTown
// @tparam      ofstream    luaFile     file where we save lua script parts.
// @tparam      Town        town        completed town object. 
// @tparam      boolean     is_main     tells if is main town.
void AddTown(std::ofstream &luaFile, Town town, bool is_main){
    
    std::string nameOfObject = "TOWN_" + factionToString(town.getFaction());
    i32 Id = town.getOwner();
    i32 X = town.getPosition().x;
    i32 Y = town.getPosition().y;

    luaFile << "instance:town(homm3lua." << nameOfObject
            << ", {x=" << X
            << ", y=" << Y
            << ", z=0}, homm3lua.PLAYER_" << Id
            << ", " << is_main << ")\n";
}

// @function    AddMine
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      Mine        mine        completed mine object. 
// @tparam      Map         map        object of map class with finised setup. 
void AddMine(std::ofstream& luaFile, Mine mine, Map &map){
    i32 x = mine.getPosition().x;
    i32 y = mine.getPosition().y;
    i32 z = mine.getPosition().z;

    i32 owner_id = mine.getOwner();

    std::string mineType = mineTypeToString(mine.getMineType());

    std::string owner = owner_id <= 0 ? "OWNER_NEUTRAL" : "PLAYER_" + std::to_string(owner_id);
    luaFile << "instance:mine(homm3lua." << mineType << ", {x=" << x << ", y=" << y << ", z=" << z << "}, homm3lua." << owner << ")\n";

}

// @function    AddRoads
// @tparam      ofstream    luaFile         file where we save lua script parts. 
// @tparam      Map         map             object of map class with finised setup.
void AddRoads(std::ofstream& luaFile, Map& map){
    
    luaFile << "-- Dynamic terrain adjustments for linear paths between towns\n";
    luaFile << "instance:terrain(function (x, y, z)\n";


    for (int x = 0; x < map.getWidth(); x++){
        for (int y = 0; y < map.getHeight(); y++){
            auto TilePtr = map.getTile(x, y);
            if (TilePtr->getIsRoad()) {            
                luaFile << "    if x == " << x << " and y == " << y << " then return nil, " << TilePtr->getTier() << " end\n";
            }
        }
    }

    luaFile << "    return nil\n"; // Default terrain
    luaFile << "end)\n";
}

// @function    AddTerrain
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      string      terrain     type of terrain, GRASS by default, See TERRAIN_*.
void AddTerrain(std::ofstream& luaFile, std::string terrain){
    luaFile << "instance:terrain(homm3lua.TERRAIN_" << terrain <<  ")\n";
}

// @function    AddTerrainTiles
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      Map         map         object of map class with finised setup.
void AddTerrainTiles(std::ofstream& luaFile, Map& map){
    luaFile << "instance:terrain(function (x, y, z)\n";

    for(int y = 0; y < map.getHeight(); y++){
        for(int x = 0; x < map.getWidth(); x++){
            auto tile = map.getTile(x, y);
            int tileZoneId = tile->getZoneId();

            if(!tile->getIsBorder())
            {
                std::string terrain = terrainToString(map.getZones()[tileZoneId]->getTerrain());
                luaFile << "if x == " << x << " and y == " << y << " then return homm3lua.TERRAIN_" << terrain << " end\n";
            }
        }
    }

    luaFile << "end)\n";
}

// @function    AddBorderObstacles
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      Map         map         object of map class with finised setup.
void AddBorderObstacles(std::ofstream& luaFile, Map& map){
    for(int y = 0; y < map.getHeight(); y++){
        for(int x = 0; x < map.getWidth(); x++){
            auto tile = map.getTile(x, y);

            std::string terrain;

            if (tile->getIsBorder() && !tile->getIsRoad()) {
                AddObstacle(luaFile, "Pine Trees", x, y, 0);
            }

            //Example Obstacles
            // Lava Flow
            // Mountain4

            //Rock_Rough10?
            // Pine Trees            
            // Mushrooms
            // Mountain
        }
    }
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
// @tparam      Creature    creature         Creature object to place.
void AddCreature(std::ofstream& luaFile, Creature creature){
    
    std::string creatureName = creature.getName();
    int x = creature.getPosition().x;
    int y = creature.getPosition().y;
    int z = creature.getPosition().z;

    int quantity = creature.getQuantity();
    std::string disposition = creature.getDisposition();
    bool never_flees = creature.getNeverFlees();
    bool does_not_grow = creature.getDoesNotGrow();
    
    luaFile << "instance:creature(homm3lua.CREATURE_" << creatureName << ", {x=" << x << ", y=" << y << ", z=" << z << "}, " << quantity << ", homm3lua.DISPOSITION_" << disposition << ", " << never_flees << ", " << does_not_grow <<  ")\n";  
}

// @function    AddResource
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      Treasure    treasure         Treasure object to place.
void AddResource(std::ofstream& luaFile, Treasure treasure){
    std::string treasureType = treasureTypeToString(treasure.getTreasureType());

    int x = treasure.getPosition().x;
    int y = treasure.getPosition().y;
    int z = treasure.getPosition().z;

    int quantity = treasure.getQuantity();

    luaFile << "instance:resource(homm3lua." << treasureType << ", {x=" << x << ", y=" << y << ", z=" << z << "}, " << quantity << ")\n";  
}

// @function    AddArtifact
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      Treasure    treasure         Treasure object to place.
void AddArtifact(std::ofstream& luaFile, Treasure treasure){
    std::string treasureType = treasureTypeToString(treasure.getTreasureType());

    int x = treasure.getPosition().x;
    int y = treasure.getPosition().y;
    int z = treasure.getPosition().z;

    luaFile << "instance:artifact(homm3lua." << treasureType << ", {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
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

// @function    AddBuildingTreasure
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      Treasure    treasure         Treasure object to place.
void AddBuildingTreasure(std::ofstream& luaFile, Treasure treasure) {
    std::string treasureType = treasureTypeToString(treasure.getTreasureType());

    int x = treasure.getPosition().x;
    int y = treasure.getPosition().y;
    int z = treasure.getPosition().z;

    luaFile << "instance:obstacle('" << treasureType << "', {x=" << x << ", y=" << y << ", z=" << z << "})\n";  

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

// @function    AddMapObjects
// @tparam      ofstream        luaFile         file where we save lua script parts. 
// @tparam      Map             map             object of map class with finised setup.
void AddMapObjects(std::ofstream &luaFile, Map& map){

    luaFile << "-- Place 1 way monoliths\n";

    MapObjects mapObjects = map.getMapObjects();

    for (auto object : mapObjects)
    {
        std::string obstacle = object.getName();
        auto pos = object.getPosition();

        luaFile << "instance:obstacle('" << obstacle << "', {x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << "})\n";
    }
}
