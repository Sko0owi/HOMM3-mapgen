#include "./ObjectPlacer.h"

#include "./Map.h"
#include "../template_info/TemplateInfo.h"
#include "../template_info/ZoneInfo.h"
#include "../template_info/ConnectionInfo.h"
#include "./template_info/MineInfo.h"
#include "./template_info/TownInfo.h"
#include "./types/int3.h"
#include "./global/Random.h"
#include "./game_info/Town.h"
#include "./game_info/Zone.h"
#include "./game_info/Mine.h"
#include "./game_info/Tile.h"
#include "./game_info/Object.h"

ObjectPlacer::ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng) : map(map), temp(temp), rng(rng) {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    objectsMap.resize(mapHeight);
    for(auto& row : objectsMap) {
        row.resize(mapWidth);
    }

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            objectsMap[y][x] = 0;
        }
    }

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            auto TilePtr = map.getTile(x, y);
            if ((TilePtr->getIsBorder() || TilePtr->getIsExtension()) && !TilePtr->getIsRoad()) {
                objectsMap[y][x] = 1;
            }
        }
    }

    std::cerr << "calculating center of zones\n";
    for(auto& zone : map.getZones()) {
        auto zonePtr = zone.second;
        
        auto centerOfZone = std::make_shared<Object>(Object(zonePtr->getPosition(), "Center Of Zone", int3(5,3,1)));

        zonePtr->addObject(centerOfZone);
        objects[zonePtr].push_back(centerOfZone);
    }

    recalculateDistances();

}

void ObjectPlacer::placeObjects() {
    std::cerr << "Placing objects\n";

    placeTowns();

    placeMines();

    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            std::cerr << objectsMap[y][x];
        }
        std::cerr << "\n";
    }
    
}

void ObjectPlacer::placeTowns() {
    std::cerr << "Placing towns\n";

    auto zones = map.getZones();

    for(auto& zone : zones) {
        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        auto zoneI = temp.getZonesI()[zoneId];

        for (auto& townI : zoneI->getTowns()) {
            
            Town town(townI);

            town.setSizeOfObject(int3(5,3,1));
            town.setPosition(zonePtr->getPosition());

            int x = town.getPosition().x;
            int y = town.getPosition().y;

            for(int x_ = max(0, x - town.getSizeOfObject().x ); x_ <= x + 1; x_++) {
                for(int y_ = max(0, y - town.getSizeOfObject().y ); y_ <= y + 1; y_++) {

                    objectsMap[y_][x_] = 2;
                    if(x_ == x - town.getSizeOfObject().x || x_ == x + 1 || y_ == y - town.getSizeOfObject().y || y_ == y + 1) {
                        objectsMap[y_][x_] = 1;
                    }
                }
            }
            
            auto townPointer = std::make_shared<Town>(town);
            zonePtr->addObject(townPointer);
            
            objects[zonePtr].push_back(townPointer);

        }
    }

    recalculateDistances();
}

bool ObjectPlacer::canPlaceObject(int3 pos, int3 size) {

    int x = pos.x;
    int y = pos.y; 

    if(y == mapHeight - 1) return false;

    if(x - size.x + 1 < 0 || y - size.y + 1 < 0) return false;

    for(int x_ = max(0, x - size.x + 1); x_ <= x ; x_++) {
        for(int y_ = max(0, y - size.y + 1); y_ <= y; y_++) {
            if(objectsMap[y_][x_] >= 1) return false;

            auto TilePtr = map.getTile(x_, y_);
            if(TilePtr->getIsRoad()) return false;
            if(TilePtr->getZoneId() != map.getTile(x, y)->getZoneId()) return false;
        }
    }

    return true;
}

bool ObjectPlacer::placeMine(MineInfo mineI, std::shared_ptr<Object> centerPtr, std::shared_ptr<Zone> zonePtr, std::map<std::shared_ptr<Zone>, std::vector<pair<pair<int,int>,std::shared_ptr<Tile>>>> &zoneTiles, bool firstBasicMine, bool deterministic) {

    int3 mineSize = getMineSize(mineI.getMineType());

    vector<int3> possiblePositions;

    if(!deterministic) {

        for(auto& tiles : zoneTiles[zonePtr]) {

            auto [pos,tile] = tiles;
            auto [x,y] = pos;

            if((!firstBasicMine || objectsDistances[centerPtr][y][x] < 15) && canPlaceObject(int3(x,y,0), mineSize)) {
                possiblePositions.push_back(int3(x,y,0));
            }
        }
    } else {    
        
        int maxMin = 0;
        auto bestPos = int3(0,0,0);
        for(auto& tiles : zoneTiles[zonePtr]) {
            auto [pos,tile] = tiles;
            auto [x,y] = pos;

            if(!canPlaceObject(int3(x,y,0), mineSize)) continue;


            int localMin = INT_MAX;
            for(auto& object : objects[zonePtr]) {
                if(object->getName() != "Mine") continue;
                localMin = min(localMin, objectsDistances[object][y][x]);
            }
            if(localMin > maxMin) {
                maxMin = localMin;
                bestPos = int3(x,y,0);
            }
        }
        possiblePositions.push_back(bestPos);
        
    }

    if(possiblePositions.empty()) {
        std::cerr << "No possible placement\n";
        return false;
    }

    int rand = rng->nextInt(0, possiblePositions.size() - 1);
    auto pos = possiblePositions[rand];

    Mine mine(mineI);

    mine.setPosition(pos);
    mine.setSizeOfObject(mineSize);

    int x = mine.getPosition().x;
    int y = mine.getPosition().y;

    for(int x_ = max(0, x - mine.getSizeOfObject().x ); x_ <= min(x + 1, mapWidth - 1) ; x_++) {
        for(int y_ = max(0, y - mine.getSizeOfObject().y ); y_ <= min(y + 1, mapHeight - 1); y_++) {

            objectsMap[y_][x_] = 3;
            if(x_ == x - mine.getSizeOfObject().x || x_ == x + 1 || y_ == y - mine.getSizeOfObject().y || y_ == y + 1) {
                objectsMap[y_][x_] = 1;
            }
        }
    }

    auto minePointer = std::make_shared<Mine>(mine);

    zonePtr->addObject(minePointer);
    objects[zonePtr].push_back(minePointer);

    return true;
}

void ObjectPlacer::placeMines() {
    std::cerr << "Placing mines\n";

    auto zones = map.getZones();

    std::map<std::shared_ptr<Zone>, std::vector<pair<pair<int,int>,std::shared_ptr<Tile>>>> zoneTiles;

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            auto TilePtr = map.getTile(x, y);
            auto TileZone = TilePtr->getZoneId();

            zoneTiles[map.getZones()[TileZone]].push_back({{x,y}, TilePtr});
        }
    }

    for(auto& zone : zones) {
        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        auto zoneI = temp.getZonesI()[zoneId];

        std::cerr << "Placing mines in zone " << zoneId << "\n";


        std::shared_ptr<Object> centerPtr = nullptr;
        for(auto& object : zone.second->getObjects()){
            if(object->getName() == "Center Of Zone") {
                centerPtr = object;
                break;
            }
        }

        if(centerPtr == nullptr) {
            std::cerr << "No center of zone\n";
            assert(false);
        }

        for(auto& mineI : zoneI->getMines()) {
            if(mineI.getMineType() == MineType::MINE_SAWMILL || mineI.getMineType() == MineType::MINE_ORE_PIT) {
                placeMine(mineI, centerPtr, zonePtr, zoneTiles, true);
                zoneI->setMaxMinesCount(zoneI->getMaxMinesCount() - 1);
                mineI.setMinCount(mineI.getMinCount() - 1);
            }
        }

        for(auto& mineI : zoneI->getMines()) {
            for(int i = 0; i < mineI.getMinCount(); i++) {
                placeMine(mineI, centerPtr, zonePtr, zoneTiles);
                zoneI->setMaxMinesCount(zoneI->getMaxMinesCount() - 1);
            }
        }


        std::vector<MineInfo> minesToPlace;
        for(auto& mineI : zoneI->getMines()) {
            minesToPlace.push_back(mineI);
        }

        int minesToPlaceCount = zoneI->getMaxMinesCount();
        while(minesToPlaceCount > 0) {

            recalculateDistances();

            int rand = rng->nextInt(0, minesToPlace.size() - 1);
            if(placeMine(minesToPlace[rand], centerPtr, zonePtr, zoneTiles, false, true) == false) {
                std::cerr << "No possible placement\n";
                break;
            }    
            minesToPlaceCount--;
                   
        }
    }

    recalculateDistances();
}

void ObjectPlacer::recalculateDistances() {

    std::cerr << "STARTING RECALC\n";
    for(auto& zone : map.getZones()) {
        auto zonePtr = zone.second;
        for(auto& object : objects[zonePtr]) {
            objectsDistances[object].clear();

            objectsDistances[object].resize(mapHeight);
            for(auto& row : objectsDistances[object]) {
                row.resize(mapWidth);
            }

            calculateShortestDistances(object);
        }
    }
}

void ObjectPlacer::calculateShortestDistances(std::shared_ptr<Object>& object) {

    auto pos = object->getPosition();
    struct Compare {
        bool operator()(const pair<int, int3>& a, const pair<int, int3>& b) {
            return a.first > b.first;
        }
    };

    priority_queue<pair<int, int3>, vector<pair<int, int3>>, Compare> pq;

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            objectsDistances[object][y][x] = INT_MAX;
        }
    }

    int dx[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[8] = {-1, 1, 0, 0, -1, 1, -1, 1};

    pq.push({0, pos});

    objectsDistances[object][pos.y][pos.x] = 0;

    while(!pq.empty()) {

        auto [dist, currentPos] = pq.top();
        pq.pop();

        if(dist > objectsDistances[object][currentPos.y][currentPos.x]) continue;

        auto [x, y, z] = currentPos;

        for(int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight) continue;

            if(objectsMap[ny][nx] >= 2) continue;

            int newDist = dist + 2;
            if(i > 3) newDist += 1;
        
            if(newDist < objectsDistances[object][ny][nx]) {
                objectsDistances[object][ny][nx] = newDist;
                pq.push({newDist, int3(nx, ny, z)});
            }
        }

    }


}
