#include "./ObjectPlacer.h"

#include "./Map.h"
#include "./Template.h"
#include "./global/Random.h"
#include "./game_info/Town.h"
#include "./game_info/Zone.h"
#include "./game_info/Mine.h"
#include "./game_info/Tile.h"
#include "./game_info/Object.h"
#include "./types/int3.h"

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
        objects.push_back(centerOfZone);
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

        for (auto& town : zoneI->getTowns()) {
            
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
            
            objects.push_back(townPointer);

        }
    }

    recalculateDistances();
}

bool ObjectPlacer::canPlaceObject(int3 pos, int3 size) {

    int x = pos.x;
    int y = pos.y; 

    if(x - size.x + 1 < 0 || y - size.y + 1 < 0) return false;

    for(int x_ = max(0, x - size.x + 1); x_ <= x ; x_++) {
        for(int y_ = max(0, y - size.y + 1); y_ <= y; y_++) {
            if(objectsMap[y_][x_] >= 1) return false;
        }
    }

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

        for(auto& mine : zoneI->getMines()) {

            int3 mineSize = getMineSize(mine.getMineType());

            vector<int3> possiblePositions;
            for(auto& tiles : zoneTiles[zonePtr]) {

                auto [pos,tile] = tiles;
                auto [x,y] = pos;

                if(objectsDistances[centerPtr][y][x] < 15 && canPlaceObject(int3(x,y,0), mineSize)) {
                    possiblePositions.push_back(int3(x,y,0));
                }
            }

            if(possiblePositions.empty()) {
                std::cerr << "No possible placement\n";
                continue;
            }

            int rand = rng->nextInt(0, possiblePositions.size() - 1);
            auto pos = possiblePositions[rand];

            mine.setPosition(pos);
            mine.setSizeOfObject(mineSize);

            int x = mine.getPosition().x;
            int y = mine.getPosition().y;

            for(int x_ = max(0, x - mine.getSizeOfObject().x ); x_ <= x + 1 ; x_++) {
                for(int y_ = max(0, y - mine.getSizeOfObject().y ); y_ <= y + 1; y_++) {

                    objectsMap[y_][x_] = 3;
                    if(x_ == x - mine.getSizeOfObject().x || x_ == x + 1 || y_ == y - mine.getSizeOfObject().y || y_ == y + 1) {
                        objectsMap[y_][x_] = 1;
                    }
                }
            }

            auto minePointer = std::make_shared<Mine>(mine);

            zonePtr->addObject(minePointer);
            objects.push_back(minePointer);

        }
    }

    recalculateDistances();
}

void ObjectPlacer::recalculateDistances() {

    std::cerr << "STARTING RECALC\n";
    for(auto& object : objects) {
        objectsDistances[object].clear();

        objectsDistances[object].resize(mapHeight);
        for(auto& row : objectsDistances[object]) {
            row.resize(mapWidth);
        }

        calculateShortestDistances(object);
    }
}

void ObjectPlacer::calculateShortestDistances(std::shared_ptr<Object> object) {

    auto pos = object->getPosition();
    

    priority_queue<pair<int3, int>> pq;

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            objectsDistances[object][y][x] = INT_MAX;
        }
    }

    int dx[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[8] = {-1, 1, 0, 0, -1, 1, -1, 1};

    pq.push({pos, 0});

    objectsDistances[object][pos.y][pos.x] = 0;

    while(!pq.empty()) {

        auto [currentPos, dist] = pq.top();
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
                pq.push({int3(nx, ny, z), newDist});
            }
        }

    }

    // std::cerr << "Distances for object " << object->getName() << "\n"; 
    // for(int y = 0; y < mapHeight; y++) {
    //     for(int x = 0; x < mapWidth; x++) {
    //         int dist = objectsDistances[object][y][x];
    //         if(dist == INT_MAX) dist = -1;
    //         fprintf(stderr,"%05d ", dist);
    //     }
    //     std::cerr << "\n";
    // }

}
