#include <bits/stdc++.h>
#include "../utils/lua_helpers.hpp"

#include "../game_info/Tile.h"
#include "../game_info/Town.h"
#include "../game_info/Mine.h"
#include "../Template.h"
#include "../RoadPlacer.h"
#include "../Map.h"
#include "./Zone.h"
#include "./global/Random.h"

using json = nlohmann::json;

void placeGateCreatures(std::ofstream& luaFile, Map& map){
    std::cerr << "Place creatures blocking gates\n";

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            auto TilePtr = map.getTile(x, y);

            if(TilePtr && TilePtr->getIsGate()){
                AddCreature(luaFile, "ARCHANGEL", x, y , 0, 100, "AGGRESSIVE", true, true);
            }
        }
    }

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            auto TilePtr = map.getTile(x, y);

            if(TilePtr->getIsGate()){
                std::cerr << "G ";  
            } 
            else if(TilePtr->getIsRoad()){    std::cerr << TilePtr->getTier() << " ";} 
            else if(TilePtr->getIsExtension()){
                std::cerr << "E ";
            } 
            else if(TilePtr->getIsBorder()){
                std::cerr << "B ";
            } 
            else {
                std::cerr << ". ";
            }
        }
        std::cerr << "\n";
    }
}

void generateLuaScript(const json& config) {
    std::ofstream luaFile("generated_script.lua");
    if (!luaFile.is_open()) {
        std::cerr << "Failed to create Lua script file." << std::endl;
        return;
    }
    
    AddHeader(luaFile);

    TemplateInfo templateInfo;
    templateInfo.deserialize(config);

    if (config["debug"])
        templateInfo.printTemplate();

    luaFile << "local instance = homm3lua.new(homm3lua.FORMAT_ROE, homm3lua.SIZE_";

    luaFile << templateInfo.getMapSize() << ")" << "\n";

    luaFile << "instance:name('" << templateInfo.getName() << "')\n";
    luaFile << "instance:description('" << templateInfo.getDescription() << "')\n";
    luaFile << "instance:difficulty(homm3lua.DIFFICULTY_" << templateInfo.getDifficulty() << ")\n\n";

    std::set<int> addedPlayers;
    std::vector<std::pair<int, int>> towns;

    RNG rng;

    rng.setSeed(12345);

    Map map(&rng);
    map.generateMap(templateInfo);

    std::cerr << "Map generated\n";
    if (config["debug"])
        map.print();
    
    AddTerrain(luaFile);
    AddTerrainTiles(luaFile, map);
    
    auto zones = map.getZones();
    for (auto& zone : zones) {
        int playerId = zone.second->getOwnerId();
        
        if(playerId == 0) continue;

        if (addedPlayers.find(playerId) == addedPlayers.end()){
            addedPlayers.insert(playerId);
            AddPlayer(luaFile, playerId);
        }

        for(auto& object : zone.second->getObjects()){
            if (auto town = std::dynamic_pointer_cast<Town>(object)) {
                AddTown(luaFile, *town);
            }

            if (auto mine = std::dynamic_pointer_cast<Mine>(object)) {
                AddMine(luaFile, *mine);
            }
        }

        

        AddHero(luaFile, zone.second);
    }   
    AddBorderObstacles(luaFile, map);

    AddRoads(luaFile, map);
    
    if (config["debug"]){
        for(auto e : towns){
            std::cerr << e.first << " " << e.second << "\n";
        }
    }

    placeGateCreatures(luaFile, map);

    string homeDir = getenv("HOME");
    cerr << "Home dir: " << homeDir << endl;
    luaFile << "instance:write('" + homeDir + "/.local/share/vcmi/Maps/test.h3m')";
    luaFile << "\n";
    
    luaFile.close();
    std::cout << "Lua script generated successfully." << std::endl;
}

void execute_lua_script(const std::string& script_name) {
    std::cerr << "Executing generated lua\n";
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
