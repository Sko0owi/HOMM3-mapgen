#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <set>
#include </usr/include/lua5.4/lua.hpp>

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

void AddTerrain(std::ofstream& luaFile){
    luaFile << "instance:terrain(homm3lua.TERRAIN_GRASS)\n";
}

void AddHeader(std::ofstream& luaFile){
        luaFile << R"(
package.cpath = package.cpath .. ';dist/?.so;../dist/?.so'
local homm3lua = require('homm3lua'))";

}


std::vector<std::pair<int, int>> GenerateSimplePath(int x1, int y1, int x2, int y2) {
    std::vector<std::pair<int, int>> path;

    while (x1 != x2) {
        path.emplace_back(x1, y1);
        x1 += (x2 > x1) ? 1 : -1;
    }

    while (y1 != y2) {
        path.emplace_back(x1, y1);
        y1 += (y2 > y1) ? 1 : -1;
    }

    return path;
}


void CreateLinearPathsToHub(std::ofstream& luaFile, const std::vector<std::pair<int, int>>& towns, int gridWidth, int gridHeight, int maxWidth = 1) {
    auto hub = std::pair(20, 8);

    luaFile << "-- Dynamic terrain adjustments for linear paths\n";
    luaFile << "instance:terrain(function (x, y, z)\n";
    luaFile << "    -- Hub point\n";
    luaFile << "    -- Hub coordinates: (" << hub.first << ", " << hub.second << ")\n";

    for (const auto& town : towns) {
        auto path = GenerateSimplePath(town.first, town.second, hub.first, hub.second);

        for (size_t i = 0; i < path.size(); ++i) {
            const auto& point = path[i];

            luaFile << "    if x == " << point.first << " and y == " << point.second << " then return nil, 1 end\n";
        }
    }

    luaFile << "    if x == " << hub.first << " and y == " << hub.second << " then return nil, 1 end\n";


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
    luaFile << "local instance = homm3lua.new(homm3lua.FORMAT_ROE, homm3lua.SIZE_";

    luaFile << config["size"].get<std::string>() << ")" << "\n";

    luaFile << "instance:name('" << config["name"] << "')\n";
    luaFile << "instance:description('" << config["description"] << "')\n";
    luaFile << "instance:difficulty(homm3lua.DIFFICULTY_" << config["difficulity"].get<std::string>() << ")\n\n";

    std::set<int> addedPlayers;
    std::vector<std::pair<int, int>> towns;
    int gridWidth = 50; // Adjust to map size
    int gridHeight = 50; // Adjust to map size

    AddTerrain(luaFile);
    for (const auto& zone : config["zones"]) {
        int playerId = zone["id"];

        if (addedPlayers.find(playerId) == addedPlayers.end()){
            addedPlayers.insert(playerId);
            AddPlayer(luaFile, playerId);
        }

        AddTown(luaFile, zone);
        AddHero(luaFile, zone);
        towns.emplace_back(zone["id"].get<int>() * 5, zone["id"].get<int>() * 5);
    }

    for(auto e : towns){
        std::cerr << e.first << " " << e.second << "\n";
    }

    CreateLinearPathsToHub(luaFile, towns, gridWidth, gridHeight, 3);

    luaFile << R"(instance:write('/home/gk/.local/share/vcmi/Maps/test.h3m'))";
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
    execute_lua_script("generated_script.lua");


    return 0;
}
