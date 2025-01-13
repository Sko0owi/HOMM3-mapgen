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
    if (terrain == "Grass") {
        return Terrain::GRASS;
    } else if (terrain == "Sand") {
        return Terrain::SAND;
    } else if (terrain == "Snow") {
        return Terrain::SNOW;
    } else if (terrain == "Swamp") {
        return Terrain::SWAMP;
    } else if (terrain == "Rough") {
        return Terrain::ROUGH;
    } else if (terrain == "Subterranean") {
        return Terrain::SUBTERRANEAN;
    } else if (terrain == "Lava") {
        return Terrain::LAVA;
    } else if (terrain == "Water") {
        return Terrain::WATER;
    } else if (terrain == "Dirt") {
        return Terrain::DIRT;
    } else if (terrain == "Rock") {
        return Terrain::ROCK;
    }
    return Terrain::DIRT;
}