#include "./GuardPlacer.h"
#include "./ObjectPlacer.h"
#include "../template_info/TemplateInfo.h"
#include "../template_info/ZoneInfo.h"
#include "../template_info/ConnectionInfo.h"
#include "./Map.h"
#include "./Tile.h"
#include "../global/Random.h"

GuardPlacer::GuardPlacer(Map &map, TemplateInfo &temp, RNG *rng) 
    : map(map), temp(temp), rng(rng) {}

void GuardPlacer::placeGuards(std::shared_ptr<ObjectPlacer> objectPlacer){
    
}

std::string GuardPlacer::getZoneDifficulty(int zoneId){
    for(auto zone : temp.getZonesI()){
        if(zone.second->getId() == zoneId){
            return zone.second->getDifficulty();
        }
    }
    return "Normal";
}

std::pair<int, int> GuardPlacer::getQuantityRange(Difficulty diff) {
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
            throw std::invalid_argument("Invalid difficulty level");
    }
}

bool GuardPlacer::doesNotGrow(Difficulty diff) {
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
            throw std::invalid_argument("Invalid difficulty level" );
    }
}

bool GuardPlacer::neverFlies(Difficulty diff) {
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
            throw std::invalid_argument("Invalid difficulty level" );
    }
}

std::pair<double, double> GuardPlacer::getGuardLevel(Difficulty diff) {
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
            throw std::invalid_argument("Invalid difficulty level" );
    }
}

std::pair<double, double> GuardPlacer::getBorderGuardLevel(Difficulty diff) {
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
            throw std::invalid_argument("Invalid difficulty level" );
    }
}

std::string GuardPlacer::getDisposition(Difficulty diff) {
    switch (diff) {
        case Difficulty::Beginner:
            return "COMPLIANT";   // Will always join the hero
        case Difficulty::Easy:
            return "FRIENDLY";    // Likely to join the hero
        case Difficulty::Normal:
            return "AGGRESSIVE";  // May join the hero
        case Difficulty::Hard:
            return "HOSTILE";     // Unlikely to join the hero
        case Difficulty::Expert:
            return "SAVAGE";      // Will never join the hero
        case Difficulty::Impossible:
            return "SAVAGE";
        default:
            throw std::invalid_argument("Invalid difficulty level");
    }
}

Difficulty GuardPlacer::stringToDifficulty(const std::string &difficulty) {
    if (difficulty == "Beginner") return Difficulty::Beginner;
    if (difficulty == "Easy") return Difficulty::Easy;
    if (difficulty == "Normal") return Difficulty::Normal;
    if (difficulty == "Hard") return Difficulty::Hard;
    if (difficulty == "Expert") return Difficulty::Expert;
    if (difficulty == "Impossible") return Difficulty::Impossible;
    return Difficulty::Invalid; 
}
