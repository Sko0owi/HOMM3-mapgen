#include <bits/stdc++.h>
#include "./luaUtils/lua_helpers.hpp"

#include "./gameInfo/Tile.h"
#include "./gameInfo/Town.h"
#include "./gameInfo/Mine.h"
#include "./gameInfo/Treasure.h"
#include "./gameInfo/Creature.h"
#include "./templateInfo/TemplateInfo.h"
#include "./placers/RoadPlacer.h"
#include "./placers/GuardPlacer.h"
#include "./Map.h"
#include "./gameInfo/Zone.h"
#include "./global/Random.h"
#include "./global/PerlinNoise.h"

#include "./CreaturesConstants.h"


using json = nlohmann::json;

void generateLuaScript(const json& config) {
    std::ofstream luaFile("generated_script.lua");
    if (!luaFile.is_open()) {
        std::cerr << "Failed to create Lua script file." << std::endl;
        return;
    }
    
    AddHeader(luaFile);

    TemplateInfo templateInfo;
    templateInfo.deserialize(config);

    if (config.value("debug", false))
        templateInfo.printTemplate();

    luaFile << "local instance = homm3lua.new(homm3lua.FORMAT_ROE, homm3lua.SIZE_";

    luaFile << templateInfo.getMapSize() << ")" << "\n";

    luaFile << "instance:name('" << templateInfo.getName() << "')\n";
    luaFile << "instance:description('" << templateInfo.getDescription() << "')\n";
    luaFile << "instance:difficulty(homm3lua.DIFFICULTY_" << templateInfo.getDifficulty() << ")\n\n";

    std::set<int> addedPlayers;


    RNG rng;
    rng.setSeed(12);


    Map map(&rng);
    map.generateMap(templateInfo);


    if (config.value("debug", false)) {
        std::cerr << "Map generated\n";
        map.print();
    }
    
    AddTerrain(luaFile);
    AddTerrainTiles(luaFile, map);


    auto zones = map.getZones();

    for (auto& zone : zones) {
        for(auto& object : zone.second->getObjects()){
            
            // add town and update players
            if (auto town = std::dynamic_pointer_cast<Town>(object)) {

                int playerId = town->getOwner();
                if (addedPlayers.find(playerId) == addedPlayers.end()){
                    addedPlayers.insert(playerId);
                    AddPlayer(luaFile, playerId);
                }

                AddTown(luaFile, *town);
            }

            // add mine
            if (auto mine = std::dynamic_pointer_cast<Mine>(object)) {
                AddMine(luaFile, *mine, map);
            }
            
            // add treasure (resource, artifact, building)
            if (auto treasure = std::dynamic_pointer_cast<Treasure>(object)) {
                string treasureType = treasureTypeToString(treasure->getTreasureType());

                if(treasureType.find("ARTIFACT") != string::npos) {
                    AddArtifact(luaFile, *treasure);
                } else if (treasureType.find("RESOURCE") != string::npos) {
                    AddResource(luaFile, *treasure);
                } else {
                    AddBuildingTreasure(luaFile, *treasure);
                }
            }
        }
    }

    AddBorderObstacles(luaFile, map);

    AddMapObjects(luaFile, map);

    AddRoads(luaFile, map);

    for(auto &zone : zones) {
        for(auto creature : zone.second->getCreatures()) {
            AddCreature(luaFile, *creature);
        }
    }

    string homeDir = getenv("HOME");

    luaFile << "instance:write('" + homeDir + "/.local/share/vcmi/Maps/test.h3m')";
    luaFile << "\n";
    
    luaFile.close();
    std::cout << "Lua script generated successfully." << std::endl;
}

void execute_lua_script(const std::string& script_name) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_dofile(L, script_name.c_str())) {
        std::cerr << "Error executing Lua script '" << script_name 
                  << "': " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
    }

    lua_close(L);
}

int main() {


    std::ifstream file("config.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open config.json file." << std::endl;
        return 1;
    }

    json config;
    try {
        file >> config;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }

    generateLuaScript(config);

    std::cout << "Executing Lua script...\n";
    execute_lua_script("generated_script.lua");
    std::cout << "Lua script executed successfully.\n";

    return 0;
}
