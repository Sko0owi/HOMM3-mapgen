#pragma once

#include <string>

enum class Terrain {
    DIRT = 0,
    SAND,
    GRASS,
    SNOW,
    SWAMP,
    ROUGH,
    SUBTERRANEAN,
    LAVA,
    WATER,
    ROCK,
};


std::string terrainToString(Terrain fac);

Terrain stringToTerrain(std::string fac);