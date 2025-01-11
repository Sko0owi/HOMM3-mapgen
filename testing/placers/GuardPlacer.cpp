#include "./GuardPlacer.h"
#include "../template_info/TemplateInfo.h"
#include "../template_info/ZoneInfo.h"
#include "../template_info/ConnectionInfo.h"
#include "./Map.h"
#include "./Tile.h"
#include "../global/Random.h"
#include "../utils/lua_helpers.hpp"

GuardPlacer::GuardPlacer(std::ofstream &luaFile, Map &map, TemplateInfo &temp, RNG *rng) 
    : luaFile(luaFile), map(map), temp(temp), rng(rng) {}

void GuardPlacer::placeGuards(){
    std::cerr << "Place creatures blocking gates\n";

    for (int i = 0; i < 7; i++){
        getZoneDifficulty(i);
    }

    for (int y = 0; y < map.getHeight(); y++)
    {
        for (int x = 0; x < map.getWidth(); x++)
        {
            auto TilePtr = map.getTile(x, y);

            if (TilePtr && (TilePtr->getIsGate() || TilePtr->getIsGuard()))
            {
                std::string difficulty = getZoneDifficulty(TilePtr->getZoneId());
                auto [min_lvl, max_lvl] = TilePtr->getIsGuard() ? getGuardLevel(difficulty) : getBorderGuardLevel(difficulty);
                double lvl = rng->nextDoubleRounded(min_lvl, max_lvl);

                auto [min_quantity, max_quantity] = getQuantityRange(difficulty);
                AddCreature(luaFile, rng->randomCreature(lvl), x, y, 0, rng->nextInt(min_quantity, max_quantity), "AGGRESSIVE", neverFlies(difficulty), doesNotGrow(difficulty));
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

std::string GuardPlacer::getZoneDifficulty(int zoneId){
    for(auto zone : temp.getZonesI()){
        if(zone.second->getId() == zoneId){
            return zone.second->getDifficulty();
        }
    }
    return "Normal";
}

std::pair<int, int> GuardPlacer::getQuantityRange(const std::string &difficulty) {
    Difficulty diff = stringToDifficulty(difficulty);

    switch (diff) {
        case Difficulty::Beginner:
            return {10, 20};
        case Difficulty::Easy:
            return {20, 40};
        case Difficulty::Normal:
            return {40, 80};
        case Difficulty::Hard:
            return {60, 120};
        case Difficulty::Expert:
            return {80, 160};
        case Difficulty::Impossible:
            return {100, 200};
        default:
            throw std::invalid_argument("Invalid difficulty level: " + difficulty);
    }
}

bool GuardPlacer::doesNotGrow(const std::string &difficulty) {
    Difficulty diff = stringToDifficulty(difficulty);
    switch (diff) {
        case Difficulty::Beginner:
        case Difficulty::Easy:
        case Difficulty::Normal:
            return true; 
        case Difficulty::Hard:
        case Difficulty::Expert:
        case Difficulty::Impossible:
            return false; 
        default:
            throw std::invalid_argument("Invalid difficulty level: " + difficulty);
    }
}

bool GuardPlacer::neverFlies(const std::string &difficulty) {
    Difficulty diff = stringToDifficulty(difficulty);
    switch (diff) {
        case Difficulty::Beginner:
        case Difficulty::Easy:
        case Difficulty::Normal:
            return false;
        case Difficulty::Hard:
        case Difficulty::Expert:
        case Difficulty::Impossible:
            return true; 
        default:
            throw std::invalid_argument("Invalid difficulty level: " + difficulty);
    }
}

std::pair<double, double> GuardPlacer::getGuardLevel(const std::string &difficulty) {
    Difficulty diff = stringToDifficulty(difficulty);

    switch (diff) {
        case Difficulty::Beginner:
            return {1.0, 2.0};
        case Difficulty::Easy:
            return {1.5, 3.0};
        case Difficulty::Normal:
            return {2.0, 4.0};
        case Difficulty::Hard:
            return {3.0, 5.0};
        case Difficulty::Expert:
            return {4.0, 6.0};
        case Difficulty::Impossible:
            return {5.0, 7.5};
        default:
            throw std::invalid_argument("Invalid difficulty level: " + difficulty);
    }
}

std::pair<double, double> GuardPlacer::getBorderGuardLevel(const std::string &difficulty) {
    Difficulty diff = stringToDifficulty(difficulty);

    switch (diff) {
        case Difficulty::Beginner:
            return {4.0, 5.0}; 
        case Difficulty::Easy:
            return {4.5, 5.5};
        case Difficulty::Normal:
            return {5.0, 6.0};
        case Difficulty::Hard:
            return {5.5, 6.5};
        case Difficulty::Expert:
            return {6.0, 7.0};
        case Difficulty::Impossible:
            return {6.5, 7.5};
        default:
            throw std::invalid_argument("Invalid difficulty level: " + difficulty);
    }
}
