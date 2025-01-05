#include <bits/stdc++.h>
#include "../utils/lua_helpers.hpp"

#include "../game_info/Tile.h"
#include "../game_info/Town.h"
#include "../Template.h"
#include "../Map.h"
#include "./Zone.h"
#include "./global/Random.h"

using json = nlohmann::json;

std::vector<std::pair<int, int>> generateSimplePath(int x1, int y1, int x2, int y2, Map& map) {
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

            //!TilePtr->getIsEdge() &&
            if (TilePtr && cameFrom.find({nx, ny}) == cameFrom.end() && 
                    (!TilePtr->getIsGate() || (TilePtr->getIsGate() && map.isMiddle(nx, ny)))) {
                q.emplace(nx, ny);
                cameFrom[{nx, ny}] = {cx, cy};
            }
        }
    }

    return path;
}

void createShotestPathsToConnected(std::ofstream& luaFile, vector<std::tuple<int, int, int, int>> &connectedPairs, Map& map, TemplateInfo& temp) {
    auto zonesI = temp.getZonesI();
    std::set<std::pair<int, int>> processedConnections;

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};



    luaFile << "-- Dynamic terrain adjustments for linear paths between towns\n";
    luaFile << "instance:terrain(function (x, y, z)\n";

    for(auto e : connectedPairs){
        auto [x1, y1, x2, y2] = e;
        auto path = generateSimplePath(x1, y1, x2, y2, map);

        for (const auto& point : path) {
            auto TilePtr = map.getTile(point.first, point.second);
            TilePtr->setIsEdge(false);

            for (int i = 0; i < 4; ++i) {
                int nx = point.first + dx[i];
                int ny = point.second + dy[i];

                auto TilePtr = map.getTile(nx, ny);

                if (TilePtr && TilePtr->getIsEdge()) {
                    TilePtr->setIsEdge(false);
                }
            }

            luaFile << "    if x == " << point.first << " and y == " << point.second << " then return nil, 3 end\n";
        }

    }
    luaFile << "    return nil\n"; // Default terrain
    luaFile << "end)\n";
}


void placeGateCreatures(std::ofstream& luaFile, Map& map){
    std::cerr << "Place creatures blocking gates\n";

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            auto TilePtr = map.getTile(x, y);

            if(TilePtr && TilePtr->getIsMiddleGate()){
                AddCreature(luaFile, "ARCHANGEL", x, y, 0, 100, "AGGRESSIVE", true, true);
            }
        }
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

        for (auto& town : zone.second->getTowns()) {
            AddTown(luaFile, zone.second, town);
        }

        AddHero(luaFile, zone.second);
    }

    auto connectedPairs = map.getConnectedPairs();
    createShotestPathsToConnected(luaFile, connectedPairs, map, templateInfo);

    AddEdgeObstacles(luaFile, map);
    
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
