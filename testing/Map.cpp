#include "./Map.h"
#include "./Zone.h"
#include "./template_info/TemplateInfo.h"
#include "./BorderPlacer.h"
#include "./ZonePlacer.h"
#include "./ObjectPlacer.h"
#include "./ObjectPlacer.h"
#include "./Faction.h"
#include "./Tile.h"
#include "./global/Random.h"
#include "./game_info/Town.h"
#include "./RoadPlacer.h"

Map::Map(RNG *rng) {
    this->rng = rng;
    this->width = 0;
    this->height = 0;
}

Map::Map(i32 width, i32 height) {
    this->width = width;
    this->height = height;
}

Map::~Map() {
}

pair<int,int> decodeMapSize(std::string MapSize) {
    if (MapSize == "SMALL") return {36, 36};
    if (MapSize == "MEDIUM") return {72, 72};
    if (MapSize == "LARGE") return {108, 108};
    return {-1,-1};
}

void Map::generateMap(TemplateInfo &temp) {

    std::string MapSize = temp.getMapSize();

    pair<int,int> width_height = decodeMapSize(MapSize);

    width = width_height.first;
    height = width_height.second;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Tiles[i][j] = std::make_shared<Tile>();
        }
    }

    class ZonePlacer zonePlacer(*this, temp, rng);

    zonePlacer.generateZones();

    class BorderPlacer borderPlacer(*this, temp);
    borderPlacer.generateBorders();
    setConnectedPairs(borderPlacer.getConnectedPairs());


    RoadPlacer roadPlacer(*this, temp);
    roadPlacer.createShotestPathsToConnected(connectedPairs);


    class ObjectPlacer objectPlacer(*this, temp, rng);

    objectPlacer.placeObjects();
}

void Map::setConnectedPairs(std::vector<std::tuple<int, int, int, int, bool, int>> connectedPairs){
    this->connectedPairs = connectedPairs;
}

vector<std::tuple<int, int, int, int, bool, int>> Map::getConnectedPairs(){
    return connectedPairs;
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

std::shared_ptr<Tile> Map::getTile(i32 x, i32 y) {
    if(0 <= x && x < width && 0 <= y && y < height)
        return Tiles[x][y];
    return nullptr;
}

bool Map::isMiddle(i32 x, i32 y){
    int no = 0;

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            auto TilePtr = getTile(nx, ny);

            if(TilePtr && TilePtr->getIsGate()){
                no++;
            }
    }

    return no > 2;
}

void Map::print() {
    std::cerr << "===================== MAP DEBUG =====================\n";
    std::cerr << "Map width: " << width << "\n";
    std::cerr << "Map height: " << height << "\n";
    for (auto& zone : zones) {
        std::cerr << "Zone Key " << zone.first << "\n";
        zone.second->printZone();
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            std::cerr << "Tile " << i << " " << j << " ZoneId: " << Tiles[i][j]->getZoneId() << "\n";
        }
    }

    std::cerr << "===================== END MAP DEBUG =====================\n";
}

