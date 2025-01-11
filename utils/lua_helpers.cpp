#include "./lua_helpers.hpp"
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
    // std::string mine = mine.getName();
    i32 x = mine.getPosition().x;
    i32 y = mine.getPosition().y;
    i32 z = mine.getPosition().z;

    i32 owner_id = mine.getOwner();

    std::string mineType = mineTypeToString(mine.getMineType());

    // std::cerr << x << " " << y << " " << z << " " << mineType << "\n";

    std::string owner = owner_id <= 0 ? "OWNER_NEUTRAL" : "PLAYER_" + std::to_string(owner_id);
    luaFile << "instance:mine(homm3lua." << mineType << ", {x=" << x << ", y=" << y << ", z=" << z << "}, homm3lua." << owner << ")\n";

    auto TilePtr = map.getTile(x-1, y+1); //todo fix mines placement or this line:DD
    TilePtr->setIsGuard(true);
}

// @function    AddRoads
// @tparam      ofstream    luaFile         file where we save lua script parts. 
// @tparam      Map         map             object of map class with finised setup.
void AddRoads(std::ofstream& luaFile, Map& map){
    luaFile << "-- Place 1 way monoliths\n";
    AddMapObjects(luaFile, map);
    
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

// @function    AddHero
// @tparam      ofstream                     luaFile     file where we save lua script parts. 
// @tparam      std::shared_ptr<Zone>        zone        shared pointer to Zone object.
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
    MapObjects mapObjects = map.getMapObjects();

    for (auto object : mapObjects)
    {
        std::string obstacle = object.getName();
        auto pos = object.getPosition();

        luaFile << "instance:obstacle('" << obstacle << "', {x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << "})\n";
    }
}

// @function    AddGuards
// @tparam      ofstream        luaFile         file where we save lua script parts. 
// @tparam      Map             map             object of map class with finised setup.
// @tparam      TemplateInfo    templateInfo    object of TemplateInfo class.
// @tparam      RNG             rng             object of RNG class.
void AddGuards(std::ofstream &luaFile, Map &map, TemplateInfo &templateInfo, RNG *rng){
    std::cerr << "Place guards\n";

    class GuardPlacer guardPlacer(map, templateInfo, rng);

    for (int y = 0; y < map.getHeight(); y++)
    {
        for (int x = 0; x < map.getWidth(); x++)
        {
            auto TilePtr = map.getTile(x, y);

            if (TilePtr && (TilePtr->getIsGate() || TilePtr->getIsGuard()))
            {
                std::string difficulty = guardPlacer.getZoneDifficulty(TilePtr->getZoneId());
                Difficulty diff = guardPlacer.stringToDifficulty(difficulty);
                auto [min_lvl, max_lvl] = TilePtr->getIsGuard() ? guardPlacer.getGuardLevel(diff) : guardPlacer.getBorderGuardLevel(diff);
                double lvl = rng->nextDoubleRounded(min_lvl, max_lvl);

                auto [min_quantity, max_quantity] = guardPlacer.getQuantityRange(diff);
                AddCreature(luaFile, rng->randomCreature(lvl), x, y, 0, rng->nextInt(min_quantity, max_quantity), guardPlacer.getDisposition(diff), guardPlacer.neverFlies(diff), guardPlacer.doesNotGrow(diff));
            }
        }
    }
}
