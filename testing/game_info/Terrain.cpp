#include "./Terrain.h"

std::string terrainToString(Terrain terrain) {
    switch (terrain) {
        case Terrain::GRASS: return "GRASS";
        case Terrain::SAND: return "SAND";
        case Terrain::SNOW: return "SNOW";
        case Terrain::SWAMP: return "SWAMP";
        case Terrain::ROUGH: return "ROUGH";
        case Terrain::SUBTERRANEAN: return "SUBTERRANEAN";
        case Terrain::LAVA: return "LAVA";
        case Terrain::WATER: return "WATER";
        case Terrain::DIRT: return "DIRT";
        case Terrain::ROCK: return "ROCK";
    }
    return "DEFAULT";
}

Terrain stringToTerrain(std::string terrain) {
    if (terrain == "GRASS") {
        return Terrain::GRASS;
    } else if (terrain == "SAND") {
        return Terrain::SAND;
    } else if (terrain == "SNOW") {
        return Terrain::SNOW;
    } else if (terrain == "SWAMP") {
        return Terrain::SWAMP;
    } else if (terrain == "ROUGH") {
        return Terrain::ROUGH;
    } else if (terrain == "SUBTERRANEAN") {
        return Terrain::SUBTERRANEAN;
    } else if (terrain == "LAVA") {
        return Terrain::LAVA;
    } else if (terrain == "WATER") {
        return Terrain::WATER;
    } else if (terrain == "DIRT") {
        return Terrain::DIRT;
    } else if (terrain == "ROCK") {
        return Terrain::ROCK;
    }
    return Terrain::DIRT;
}