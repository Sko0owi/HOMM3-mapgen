#pragma once

#include "../global/Global.h"
#include "../global/Random.h"
#include <nlohmann/json.hpp> 


enum class Terrain;

class TownInfo;
class MineInfo;

using json = nlohmann::json;

class ConnectionInfo;
class ZoneInfo;


using ZoneI = std::shared_ptr<ZoneInfo>;
using ZonesI = std::map<i32, ZoneI>;


class TemplateInfo {
public:

    TemplateInfo();

    ZonesI & getZonesI();

    void setName(std::string name);
    void setDescription(std::string description);
    void setMapSize(pair<int,int> mapSize);
    void setDifficulty(std::string difficulty);

    std::string getName();
    std::string getDescription();
    pair<int,int> getMapSize();
    std::string getDifficulty();

    void deserialize(const json& config, RNG *rng);
    void printTemplate();

    template <typename T>
    static T getOrError(const nlohmann::json& j, const std::string& key);

private:
    std::string name;
    std::string description;
    std::string difficulty; 
    pair<int,int> mapSize;
    ZonesI zonesI;
};

template <typename T>
T TemplateInfo::getOrError(const nlohmann::json& j, const std::string& key) {
     if (j.contains(key)) { // Check if the key exists
        try {
            return j.at(key).get<T>(); // Safely attempt to get the value
        } catch (const std::exception& e) {
            throw std::runtime_error("Error reading '" + key + "': " + e.what());
        }
    }
    throw std::runtime_error("Key '" + key + "' does not exist in the JSON object.");
}
