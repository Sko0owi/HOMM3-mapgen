#include "./Map.h"
#include "./Zone.h"
#include "./Template.h"
#include "./ZonePlacer.h"

Map::Map() {
    width = 0;
    height = 0;
}

Map::Map(int width, int height) {
    this->width = width;
    this->height = height;
}

Map::~Map() {
}

pair<int,int> decodeMapSize(std::string MapSize) {
    if (MapSize == "SMALL") return {36, 36};
    if (MapSize == "MEDIUM") return {72, 72};
    return {-1,-1};
}

void Map::initialize(TemplateInfo &temp) {

    std::string MapSize = temp.getMapSize();

    pair<int,int> width_height = decodeMapSize(MapSize);

    width = width_height.first;
    height = width_height.second;

    generate();
}

MapZones & Map::getZones() {
    return zones;
}

i32 Map::getWidth() {
    return width;
}

i32 Map::getHeight() {
    return height;
}

void Map::generate() {
    class ZonePlacer zonePlacer;
    zonePlacer.generateZones();
}

void Map::print() {
    for (auto& zone : zones) {
        std::cerr << zone.first << "\n";
    }
}


