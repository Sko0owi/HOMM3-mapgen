#include "lua_helpers.hpp"
#include <sstream>

#include "../game_info/Tile.h"
#include "../template_info/TemplateInfo.h"
#include "../template_info/ZoneInfo.h"
#include "../template_info/ConnectionInfo.h"
#include "./Map.h"
#include "./game_info/Zone.h"
#include "./game_info/Terrain.h"
#include "./game_info/Town.h"
#include "./game_info/Treasure.h"
#include "./game_info/Mine.h"
#include "../placers/ObjectPlacer.h"
#include "../global/Random.h"
#include "../placers/BorderPlacer.h"
#include "../placers/RoadPlacer.h"




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
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      mine             type of mine, See MINE_*.
// @tparam      integer     x                position on x axis of mine.
// @tparam      integer     y                position on y axis of mine.
// @tparam      integer     z                position on z axis of mine.
// @tparam      integer     owner_id         owner of mine, by default -1, which means neutral.
void AddMine(std::ofstream& luaFile, Mine mine){
    // std::string mine = mine.getName();
    i32 x = mine.getPosition().x;
    i32 y = mine.getPosition().y;
    i32 z = mine.getPosition().z;

    i32 owner_id = mine.getOwner();

    std::string mineType = mineTypeToString(mine.getMineType());

    std::string owner = owner_id <= 0 ? "OWNER_NEUTRAL" : "PLAYER_" + std::to_string(owner_id);
    luaFile << "instance:mine(homm3lua." << mineType << ", {x=" << x << ", y=" << y << ", z=" << z << "}, homm3lua." << owner << ")\n";
}

void AddRoads(std::ofstream& luaFile, Map& map, std::shared_ptr<ObjectPlacer> objectPlacer, RNG *rng){
    luaFile << "-- Place 1 way monoliths\n";
    AddMapObjects(luaFile, map, objectPlacer, rng);
    
    luaFile << "-- Dynamic terrain adjustments for linear paths between towns\n";
    luaFile << "instance:terrain(function (x, y, z)\n";


    for (int x = 0; x < map.getWidth(); x++){
        for (int y = 0; y < map.getHeight(); y++){
            auto TilePtr = map.getTile(y, x);
            if (TilePtr->getIsRoad()) {            
                luaFile << "    if x == " << y << " and y == " << x << " then return nil, " << TilePtr->getTier() << " end\n";
            }
        }
    }

    luaFile << "    return nil\n"; // Default terrain
    luaFile << "end)\n";
}

// @function    AddHero
// @tparam      ofstream    luaFile     file where we save lua script parts. 
// @tparam      json        zone        zone description from json, like player id, hero xyz position.
void AddHero(std::ofstream& luaFile, std::shared_ptr<Zone>& zone) {
    std::string hero = "HERO_CHRISTIAN";
    i32 Id = 1;
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

            if ((tile->getIsBorder() || tile->getIsExtension()) && !tile->getIsRoad()) {
                AddObstacle(luaFile, "Pine Trees", x, y, 0);
            }

            //Example Obstacles
            // Lava Flow
            // Mountain4

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

// @function    AddResource
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      mine             type of mine, See RESOURCE_*.
// @tparam      integer     x                position on x axis of resource.
// @tparam      integer     y                position on y axis of resource.
// @tparam      integer     z                position on z axis of resource.
// @tparam      integer     quantity         quantity of resource.
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
// @tparam      string      artifact         type of mine, See ARTIFACT_*.
// @tparam      integer     x                position on x axis of artifact.
// @tparam      integer     y                position on y axis of artifact.
// @tparam      integer     z                position on z axis of artifact.
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

// @function    AddSign
// @tparam      ofstream    luaFile          file where we save lua script parts. 
// @tparam      string      text             text on sign.
// @tparam      integer     x                position on x axis of sign.
// @tparam      integer     y                position on y axis of sign.
// @tparam      integer     z                position on z axis of sign.
void AddSign(std::ofstream& luaFile, std::string text, int x, int y, int z){
    luaFile << "instance:sign('" << text << "', {x=" << x << ", y=" << y << ", z=" << z << "})\n";  
}

std::vector<std::pair<int, int>> getValidTiles(int zoneId, Map& map, std::shared_ptr<ObjectPlacer> objectPlacer, Object object){
    std::vector<std::pair<int, int>> tiles;
    auto objectsMap = objectPlacer->getObjectsMap();


    for (int x = 1; x < map.getWidth(); x++)
    {
        for (int y = 1; y < map.getWidth(); y++){
            auto TilePtr = map.getTile(x, y);

            if(TilePtr->getZoneId() == zoneId && !TilePtr->getIsBorder() && !TilePtr->getIsExtension() && !TilePtr->getIsRoad() && objectsMap[y][x] == 0){
                bool canPlace = true;
                for (int x_ = max(0, x - object.getSizeOfObject().x); x_ <= min(x + 1, map.getWidth() - 1); x_++)
                {
                    for (int y_ = max(0, y - object.getSizeOfObject().y); y_ <= min(y + 1, map.getHeight() - 1); y_++)
                    {
                        if (x_ == x - object.getSizeOfObject().x || x_ == x + 1 || y_ == y - object.getSizeOfObject().y || y_ == y + 1)
                        {
                            if(objectsMap[y_][x_] == 1){ //We want to keep at least 1 tile separation
                                canPlace = false;
                                break;
                            }
                        }
                    }
                }
                
                if(canPlace)
                    tiles.emplace_back(x, y);
            }
        }
    }
    
    return tiles;
}

std::shared_ptr<Zone> getZoneFromId(int zoneId, Map &map){
    auto zones = map.getZones();
    for (auto& zone : zones) {
        if(zone.second->getId() == zoneId)
            return zone.second;
    }
    return nullptr;
}

// @function    AddMapObjects
// @tparam      ofstream        luaFile         file where we save lua script parts. 
// @tparam      Map             map             object of map class with finised setup.
// @tparam      ObjectPlacer    objectPlacer    object with resources and buildings.
// @tparam      RNG             rng             rng object.
void AddMapObjects(std::ofstream &luaFile, Map& map, std::shared_ptr<ObjectPlacer> objectPlacer, RNG *rng){
    MapObjects mapObjects = map.getMapObjects();
    auto objectsMap = objectPlacer->getObjectsMap();
    std::vector<std::tuple<int, int, int, int, bool, int>> connectedPairs;

    RoadPlacer roadPlacer(map);

    for (auto object : mapObjects)
    {
        std::string obstacle = object.getName();
        std::cerr << "NAME " << obstacle << "\n";
        auto pos = object.getPosition();

        auto TilePtr = map.getTile(pos.x, pos.y);
        int outerXX1 = pos.x, outerYY1 = pos.y;

        if (TilePtr->getIsBorder() || TilePtr->getIsExtension() || TilePtr->getIsRoad() || objectsMap[pos.y][pos.x] > 0)
        {
            std::vector<std::pair<int, int>> zone1Tiles = getValidTiles(TilePtr->getZoneId(), map, objectPlacer, object);
            int rand = rng->nextInt(0, zone1Tiles.size() - 1);
            std::tie(outerXX1, outerYY1) = zone1Tiles[rand];
            pos = int3(outerXX1, outerYY1, 0);
        }

        int x = pos.x;
        int y = pos.y;

        for (int x_ = max(0, x - object.getSizeOfObject().x); x_ <= min(x + 1, map.getWidth() - 1); x_++)
        {
            for (int y_ = max(0, y - object.getSizeOfObject().y); y_ <= min(y + 1, map.getHeight() - 1); y_++)
            {

                objectsMap[y_][x_] = 3;
                if (x_ == x - object.getSizeOfObject().x || x_ == x + 1 || y_ == y - object.getSizeOfObject().y || y_ == y + 1)
                {
                    objectsMap[y_][x_] = 1;
                }
            }
        }

        luaFile << "instance:obstacle('" << obstacle << "', {x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << "})\n";

        //Logic to connect tp with zone
        std::shared_ptr<Zone> zone1 = getZoneFromId(map.getTile(x, y)->getZoneId(), map);

        int XX1 = zone1->getPosition().x;
        int YY1 = zone1->getPosition().y;

        connectedPairs.emplace_back(x, y, XX1, YY1, true, rng->nextInt(1, 3)); // Castle1 -> Connect1
        if (obstacle.find("Entrance") != std::string::npos){ // We placed Entrance to monolith
            TilePtr = map.getTile(pos.x, pos.y+1);
            TilePtr->setIsGate(true);
        }
    }
    roadPlacer.createShotestPathsToConnected(connectedPairs);
}