#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <set>
#include </usr/include/lua5.4/lua.hpp>

using json = nlohmann::json;

void generateLuaScript(const json& config) {
    std::ofstream luaFile("generated_script.lua");
    if (!luaFile.is_open()) {
        std::cerr << "Failed to create Lua script file." << std::endl;
        return;
    }

    luaFile << R"(
package.cpath = package.cpath .. ';dist/?.so;../dist/?.so'
local homm3lua = require('homm3lua')
local instance = homm3lua.new(homm3lua.FORMAT_ROE, homm3lua.SIZE_SMALL)

)";

    luaFile << "instance:name('" << config["name"] << "')\n";
    luaFile << "instance:description('" << config["description"] << "')\n";
    luaFile << "instance:difficulty(homm3lua.DIFFICULTY_HARD)\n\n";

    std::set<int> addedPlayers;

    for (const auto& zone : config["zones"]) {
        int playerId = zone["id"];
        std::string player = "PLAYER_" + std::to_string(playerId);

        if (addedPlayers.find(playerId) == addedPlayers.end()) {
            luaFile << "instance:player(homm3lua." << player << ")\n";
            addedPlayers.insert(playerId);
        }
        
        std::cerr << zone << " " << zone["id"]<< "\n";
        luaFile << "-- Zone " << zone["id"] << "\n";
        luaFile << "instance:town(homm3lua." << zone["player"].get<std::string>()  
                << ", {x=" << (zone["id"].get<int>() * 5)
                << ", y=" << (zone["id"].get<int>() * 5) 
                << ", z=0}, homm3lua.PLAYER_" << zone["id"] 
                << ", true)\n\n";
    }

    // TODO MODIFY WHERE DO YOU WANT TO STORE MAP, THIS IS LOCATION FOR VCMIEDITOR
    luaFile << R"(instance:write('/home/gk/.local/share/vcmi/Maps/test.h3m'))";
    
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
