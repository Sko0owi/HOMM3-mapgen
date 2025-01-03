#include <bits/stdc++.h>
#include "../utils/lua_helpers.hpp"

#include "../game_info/Tile.h"
#include "../game_info/Town.h"
#include "../Template.h"
#include "../Map.h"
#include "./Zone.h"
#include "./global/Random.h"

using json = nlohmann::json;

std::vector<std::pair<int, int>> GenerateSimplePath(int x1, int y1, int x2, int y2, Map& map) {
    std::vector<std::pair<int, int>> path;

    if (x1 == x2 && y1 == y2) {
        path.emplace_back(x1, y1);
        return path;
    }

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    std::queue<std::pair<int, int>> q;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;

    q.emplace(x1, y1);
    cameFrom[{x1, y1}] = {-1, -1}; 

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        if (cx == x2 && cy == y2) {
            while (cameFrom[{cx, cy}] != std::make_pair(-1, -1)) {
                path.emplace_back(cx, cy);
                std::tie(cx, cy) = cameFrom[{cx, cy}];
            }
            path.emplace_back(x1, y1);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            auto TilePtr = map.getTile(nx, ny);
            if (TilePtr && !TilePtr->getIsEdge() && cameFrom.find({nx, ny}) == cameFrom.end()) {
                q.emplace(nx, ny);
                cameFrom[{nx, ny}] = {cx, cy};
            }
        }
    }

    return path;
}

void CreateShotestPathsToConnected(std::ofstream& luaFile, const std::vector<std::pair<int, int>>& towns, Map& map, TemplateInfo& temp) {
    auto zonesI = temp.getZonesI();
    std::set<std::pair<int, int>> processedConnections;


    luaFile << "-- Dynamic terrain adjustments for linear paths between towns\n";
    luaFile << "instance:terrain(function (x, y, z)\n";

    for (size_t i = 0; i < towns.size(); ++i) {
        for (size_t j = i + 1; j < towns.size(); ++j) {
            const auto& townA = towns[i];
            const auto& townB = towns[j];

            auto tileA = map.getTile(townA.first, townA.second);
            auto tileB = map.getTile(townB.first, townB.second);

            int zoneA = tileA->getZoneId();
            int zoneB = tileB->getZoneId();

            auto connectionPair = std::make_pair(std::min(zoneA, zoneB), std::max(zoneA, zoneB));

            if (processedConnections.find(connectionPair) != processedConnections.end()) {
                continue;
            }

            processedConnections.insert(connectionPair);

            auto connections = zonesI[zoneA]->getConnections();
            bool isConnected = false;
            for(auto c : connections){
                if ((c.getZoneA() == zoneA && c.getZoneB() == zoneB) ||
                    (c.getZoneA() == zoneB && c.getZoneB() == zoneA)) {
                    isConnected = true;
                }
            }

            if (!isConnected) continue;

            auto path = GenerateSimplePath(townA.first, townA.second, townB.first, townB.second, map);
            for (const auto& point : path) {
                luaFile << "    if x == " << point.first << " and y == " << point.second << " then return nil, 3 end\n";
            }
        }
    }

    luaFile << "    return nil\n"; // Default terrain
    luaFile << "end)\n";
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
    AddEdgeObstacles(luaFile, map);
    
    auto zones = map.getZones();
    for (auto& zone : zones) {
        int playerId = zone.second->getOwnerId();
        if(playerId == 0){
            i32 X = zone.second->getPosition().x;
            i32 Y = zone.second->getPosition().y;
            towns.emplace_back(X, Y);
            continue;
        }
            
        if (addedPlayers.find(playerId) == addedPlayers.end()){
            addedPlayers.insert(playerId);
            AddPlayer(luaFile, playerId);
        }

        if(zone.second->getTowns().size() > 0){
            for (auto& town : zone.second->getTowns()) {
                AddTown(luaFile, zone.second, town);
                i32 X = zone.second->getPosition().x;
                i32 Y = zone.second->getPosition().y;
                towns.emplace_back(X, Y);
            }
        } else{
            i32 X = zone.second->getPosition().x;
            i32 Y = zone.second->getPosition().y;
            towns.emplace_back(X, Y);
        }
        AddHero(luaFile, zone.second);
    }

    CreateShotestPathsToConnected(luaFile, towns, map, templateInfo);
    if (config["debug"]){
        for(auto e : towns){
            std::cerr << e.first << " " << e.second << "\n";
        }
    }

    //TEST STUFF
    AddArtifact(luaFile, "PENDANT_OF_COURAGE", 8, 1, 0);
    AddObstacle(luaFile, "Rock", 7, 1, 0);
    AddSign(luaFile, "FUNNY CATS", 8, 2, 0);
    AddMine(luaFile, "GOLD_MINE", 10, 4, 0);
    AddMine(luaFile, "GEM_POND", 15, 4, 0, 2);
    AddResource(luaFile, "CRYSTAL", 7, 3, 0, 10);
    AddCreature(luaFile, "ARCHANGEL", 12, 4, 0, 100, "AGGRESSIVE", true, true);

    string homeDir = getenv("HOME");
    cerr << "Home dir: " << homeDir << endl;
    luaFile << "instance:write('" + homeDir + "/.local/share/vcmi/Maps/test.h3m')";
    luaFile << "\n";
    
    luaFile.close();
    std::cout << "Lua script generated successfully." << std::endl;
}

void execute_lua_script(const std::string& script_name) {
    lua_State* L = luaL_newstate();
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
