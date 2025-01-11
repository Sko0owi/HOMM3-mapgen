#include <bits/stdc++.h>
#include "../utils/lua_helpers.hpp"

#include "./game_info/Tile.h"
#include "./game_info/Town.h"
#include "./game_info/Mine.h"
#include "./game_info/Treasure.h"
#include "./template_info/TemplateInfo.h"
#include "./placers/RoadPlacer.h"
#include "./Map.h"
#include "./game_info/Zone.h"
#include "./global/Random.h"

#include "./CreaturesConstants.h"


using json = nlohmann::json;

void placeGateCreatures(std::ofstream& luaFile, Map& map, RNG *rng){
    std::cerr << "Place creatures blocking gates\n";

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            auto TilePtr = map.getTile(x, y);

            if(TilePtr && TilePtr->getIsGate()){
                AddCreature(luaFile, rng->randomCreature(7), x, y , 0, 100, "AGGRESSIVE", true, true);
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

    if (config.value("debug", false))
        templateInfo.printTemplate();

    luaFile << "local instance = homm3lua.new(homm3lua.FORMAT_ROE, homm3lua.SIZE_";

    luaFile << templateInfo.getMapSize() << ")" << "\n";

    luaFile << "instance:name('" << templateInfo.getName() << "')\n";
    luaFile << "instance:description('" << templateInfo.getDescription() << "')\n";
    luaFile << "instance:difficulty(homm3lua.DIFFICULTY_" << templateInfo.getDifficulty() << ")\n\n";

    std::set<int> addedPlayers;
    std::vector<std::pair<int, int>> towns;

    RNG rng;

    rng.setSeed(12);

    Map map(&rng);
    auto objectPlacer = map.generateMap(templateInfo);

    std::cerr << "Map generated\n";
    if (config.value("debug", false))
        map.print();
    
    AddTerrain(luaFile);
    AddTerrainTiles(luaFile, map);
    
    auto zones = map.getZones();
    for (auto& zone : zones) {
        for(auto& object : zone.second->getObjects()){
            if (auto town = std::dynamic_pointer_cast<Town>(object)) {
                int playerId = town->getOwner();
                if (addedPlayers.find(playerId) == addedPlayers.end()){
                    addedPlayers.insert(playerId);
                    AddPlayer(luaFile, playerId);
                }
            }
        }

        for(auto& object : zone.second->getObjects()){
            if (auto town = std::dynamic_pointer_cast<Town>(object)) {
                AddTown(luaFile, *town);
            }

            if (auto mine = std::dynamic_pointer_cast<Mine>(object)) {
                AddMine(luaFile, *mine);
            }

            if (auto treasure = std::dynamic_pointer_cast<Treasure>(object)) {
                std::cerr << "WE ARE GONNA BE RICH!\n";
                string treasureType = treasureTypeToString(treasure->getTreasureType());
                if(treasureType[0] == 'A'){
                    AddArtifact(luaFile, *treasure);
                } else {
                    AddResource(luaFile, *treasure);
                }
            }
        }


        // std::string ehh = "ammo cart;";
        // AddArtifact(luaFile, Treasure(TreasureType::ARTIFACT_LADYBIRD_OF_LUCK, 0, zone.second->getPosition(), ehh));

        AddHero(luaFile, zone.second);
    }   

    AddBorderObstacles(luaFile, map);

    AddRoads(luaFile, map, objectPlacer, &rng);
    
    if (config.value("debug", false)){
        for(auto e : towns){
            std::cerr << e.first << " " << e.second << "\n";
        }
    }

    // for (int i = 0; i < 8; i++){
    //     AddObstacle(luaFile, "Monolith One Way Entrance" + std::to_string(i), 10, 2 * i, 0);
    // }

    // AddObstacle(luaFile, "Arena", 12, 2, 0);
    // AddObstacle(luaFile, "Faerie Ring_Dirt", 12, 4, 0);
    // AddObstacle(luaFile, "Fountain of Fortune", 12, 6, 0);
    // AddObstacle(luaFile, "Fountain of Youth", 12, 8, 0);
    // AddObstacle(luaFile, "Idol of Fortune", 12, 10, 0);
    // AddObstacle(luaFile, "Learning Stone", 12, 12, 0);
    // AddObstacle(luaFile, "Library of Enlightenment", 12, 14, 0);
    // AddObstacle(luaFile, "Marletto Tower", 12, 16, 0);
    // AddObstacle(luaFile, "Mercenary Camp", 12, 18, 0);
    // AddObstacle(luaFile, "Oasis", 12, 18, 0);
    // AddObstacle(luaFile, "Rally Flag", 12, 20, 0);
    // AddObstacle(luaFile, "School of Magic", 12, 24, 0);

    placeGateCreatures(luaFile, map, &rng);

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
