#include "./BorderPlacer.h"
#include "./ObjectPlacer.h"
#include "../Map.h"
#include "../gameInfo/Zone.h"
#include "../gameInfo/Tile.h"
#include "../templateInfo/TemplateInfo.h"
#include "../templateInfo/ZoneInfo.h"
#include "../templateInfo/ConnectionInfo.h"
#include "../global/Random.h"
#include"../types/int3.h"
#include "../gameInfo/Object.h"

BorderPlacer::BorderPlacer(Map &map, TemplateInfo &temp, RNG *rng, std::shared_ptr<ObjectPlacer> objectPlacer) : map(map), temp(temp), rng(rng), objectPlacer(objectPlacer)
{
    mapWidth = 0;
    mapHeight = 0;
    monolithCount = 0;
}


void BorderPlacer::generateBorders() {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    determineZoneBorders();
    connectZones();
    setWideConnections();
}

bool BorderPlacer::areConnected(int ZoneA, int ZoneB){
    auto zonesI = temp.getZonesI();

    for(auto e : zonesI){
        for(auto c : e.second->getConnections()){
            if ((c.getZoneA() == ZoneA && c.getZoneB() == ZoneB) ||
                (c.getZoneA() == ZoneB && c.getZoneB() == ZoneA)) {
                return true;
            }
        }
    }
    return false;
}

int BorderPlacer::getTier(int ZoneA, int ZoneB){
    auto zonesI = temp.getZonesI();

    for(auto e : zonesI){
        for(auto c : e.second->getConnections()){
            if ((c.getZoneA() == ZoneA && c.getZoneB() == ZoneB) ||
                (c.getZoneA() == ZoneB && c.getZoneB() == ZoneA))
            {
                return c.getTier();
            }
        }
    }
    return 0;
}

std::string BorderPlacer::getType(int ZoneA, int ZoneB){
    auto zonesI = temp.getZonesI();

    for(auto e : zonesI){
        for(auto c : e.second->getConnections()){
            if ((c.getZoneA() == ZoneA && c.getZoneB() == ZoneB) ||
                (c.getZoneA() == ZoneB && c.getZoneB() == ZoneA))
            {
                return c.getType();
            }
        }
    }
    return "narrow";
}

bool BorderPlacer::isMapBorder(int x, int y){

    for (int i = 0; i < 8; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];

        auto NeighborTilePtr = map.getTile(nx, ny);

        if(!NeighborTilePtr){
            return true;
        }
    }

    return false;
}

void BorderPlacer::findOuter(int X, int Y, int *outerX1, int *outerY1, int *outerX2, int *outerY2, int zone1Id, int zone2Id){
    using Node = std::tuple<int, int, int, int, double>; // x, y, fromx, fromy, dist
    auto objectsMap = objectPlacer->getObjectsMap();
    
    auto cmp = [](const Node &a, const Node &b) {
        return std::get<4>(a) > std::get<4>(b); // Comparator for Node
    };

    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);
    std::vector<std::vector<double>> distance(mapHeight, std::vector<double>(mapWidth, std::numeric_limits<double>::max()));

    const int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    const int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};

    pq.emplace(X, Y, -1, -1, 0.0);
    distance[Y][X] = 0;

    while (!pq.empty()) {
        auto [cx, cy, fromx, fromy, dist] = pq.top();
        pq.pop();

        if (dist > distance[cy][cx]) continue;

        for (int i = 0; i < 8; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            double delta = sqrt((nx - cx) * (nx - cx) + (ny - cy) * (ny - cy));

            if (nx < 0 || ny < 0 || nx >= mapWidth || ny >= mapHeight) continue;
        
            auto TilePtr = map.getTile(nx, ny);
            int ZoneC = -1;
            if(TilePtr)
                ZoneC = TilePtr->getZoneId();

            int newDist = dist + delta;

            if (newDist < distance[ny][nx])
                distance[ny][nx] = newDist;
            else 
                continue;
            

            if(zone1Id == ZoneC && !TilePtr->getIsBorder() && objectsMap[ny][nx] <= 1){ // We didn't get out of zone
                pq.emplace(nx, ny, -1, -1, newDist);
            } else if (zone2Id == ZoneC && !TilePtr->getIsBorder() && objectsMap[nx][ny] <= 1) { // We found good zone 
                (*outerX1) = fromx;
                (*outerY1) = fromy;
                (*outerX2) = nx;
                (*outerY2) = ny;
                return;
            }
            else if (((zone1Id == ZoneC && TilePtr->getIsBorder()) || (zone2Id == ZoneC && TilePtr->getIsBorder())) && fromx != -1)
            { // Travelling through Border
                pq.emplace(nx, ny, fromx, fromy, newDist);
            }
            else
            { // We want to stay in
                if(ZoneC == zone1Id || ZoneC == zone2Id)
                    pq.emplace(nx, ny, cx, cy, newDist);
            }
        }
    }
}

ConnectedPoints BorderPlacer::getConnectedPairs(){
    return connectedPairs;
}

MapObjects BorderPlacer::getMapObjects(){
    return mapObjects;
}

std::vector<std::pair<int, int>> BorderPlacer::getValidTiles(int zoneId, Object object){
    std::vector<std::pair<int, int>> tiles;
    auto objectsMap = objectPlacer->getObjectsMap();

    for (int x = 1; x < map.getWidth(); x++)
    {
        for (int y = 1; y < map.getHeight() - 1; y++){
            auto TilePtr = map.getTile(x, y);
            if(TilePtr->getZoneId() == zoneId && !TilePtr->getIsBorder() && !TilePtr->getIsRoad() && !TilePtr->getIsGate() && objectsMap[y][x] == 0){
                bool canPlace = true;
                for (int x_ = max(0, x - object.getSizeOfObject().x); x_ <= min(x + 1, map.getWidth() - 1); x_++)
                {
                    for (int y_ = max(0, y - object.getSizeOfObject().y); y_ <= min(y + 1, map.getHeight() - 1); y_++)
                    {
                        if (x_ == x - object.getSizeOfObject().x || x_ == x + 1 || y_ == y - object.getSizeOfObject().y || y_ == y + 1)
                        {
                            if(objectsMap[y_][x_] >= 1){ //We want to keep at least 1 tile separation
                                canPlace = false;
                                break;
                            }
                        }
                    }
                }
                
                if(canPlace)
                    tiles.emplace_back(x, y);
            }
        }
    }

    objectPlacer->setObjectsMap(objectsMap);
    return tiles;
}

void BorderPlacer::placeMonolith(Object object){
    auto objectsMap = objectPlacer->getObjectsMap();

    int x = object.getPosition().x;
    int y = object.getPosition().y;

    int sx = object.getSizeOfObject().x;

    objectsMap[y][x] = 3;
    objectsMap[y+1][x] = 1;
    
    if(sx > 1){
        objectsMap[y][x+1] = 7;
    }

    objectPlacer->setObjectsMap(objectsMap);
}

void BorderPlacer::connectZones() {
    if (debug) std::cerr << "Finding zones conenction points\n";
    auto zones = map.getZones();
    std::set<std::pair<int, int>> processedConnections;

    for (auto& zone1 : zones) {
        for (auto& zone2 : zones){
            int zone1Id = zone1.second->getId();
            int zone2Id = zone2.second->getId();
            
            if (zone1Id == zone2Id) continue;

            auto connectionPair = std::make_pair(std::min(zone1Id, zone2Id), std::max(zone1Id, zone2Id));

            if(areConnected(zone1Id, zone2Id) && processedConnections.find(connectionPair) == processedConnections.end()){
                if(debug)
                    std::cerr << "CONNECTING " << zone1Id << " " << zone2Id << "\n";

                processedConnections.insert(connectionPair);
                int X = zone1.second->getPosition().x;
                int Y = zone1.second->getPosition().y;
                int XX = zone2.second->getPosition().x;
                int YY = zone2.second->getPosition().y;

                int tier = getTier(zone1Id, zone2Id);
                std::string type = getType(zone1Id, zone2Id);

                int outerX1 = -1, outerY1 = -1, outerX2 = -1, outerY2 = -1;
                findOuter(X, Y, &outerX1, &outerY1, &outerX2, &outerY2, zone1Id, zone2Id);
                // OuterXi -> outer X coord from zonei
                // OuterYi -> outer Y coord from zone1
                // castle -> bool does have castle
                // tier -> tier of road

                if(outerX1 == -1 || outerX2 == -1 || type == "monolith"){ // Didn't find connection or it is monolith type -> add Monoliths
                    //Create first entrance
                    Object entrance(int3 (0, 0, 0), "Monolith One Way Entrance" + std::to_string(monolithCount));
                    int3 size = monolithCount > 3 ? int3(2, 1, 0) : int3(1, 1, 0);
                    entrance.setSizeOfObject(size);
                    
                    std::vector<std::pair<int, int>> zone1Tiles = getValidTiles(zone1Id, entrance);

                    int rand = rng->nextInt(0, zone1Tiles.size() - 1);
                    auto [outerXXX1, outerYYY1] = zone1Tiles[rand];

                    outerX1 = outerXXX1;
                    outerY1 = outerYYY1;

                    entrance.setPosition(int3(outerX1, outerY1, 0)); // Fix position
                    connectedPairs.emplace_back(X, Y, outerX1, outerY1, true, rng->nextInt(1,3)); //Castle1 -> Entrance1
                    map.getTile(outerX1, outerY1+1)->setIsGuard(true);
                    map.getTile(outerX1, outerY1+1)->setIsBorder(false);

                    placeMonolith(entrance);
                    mapObjects.emplace_back(entrance);

                    //Create first exit
                    Object exit(int3 (0, 0, 0), "Monolith One Way Exit" + std::to_string(monolithCount));
                    size = monolithCount > 3 ? int3(2, 1, 0) : int3(1, 1, 0);
                    exit.setSizeOfObject(size); 

                    std::vector<std::pair<int, int>> zone2Tiles = getValidTiles(zone2Id, exit);
                    
                    rand = rng->nextInt(0, zone2Tiles.size() - 1);
                    auto [outerXXX2, outerYYY2] = zone2Tiles[rand];

                    outerX2 = outerXXX2;
                    outerY2 = outerYYY2;

                    exit.setPosition(int3(outerX2, outerY2, 0)); // Fix position
                    connectedPairs.emplace_back(XX, YY, outerX2, outerY2, true, rng->nextInt(1,3)); //Castle2 -> Exit1

                    placeMonolith(exit);
                    mapObjects.emplace_back(exit);

                    //Create second entrance
                    Object entrance2(int3 (0, 0, 0), "Monolith One Way Entrance" + std::to_string(monolithCount + 1));
                    size = monolithCount > 2 ? int3(2, 1, 0) : int3(1, 1, 0);
                    entrance2.setSizeOfObject(size);
                    
                    zone2Tiles = getValidTiles(zone2Id, entrance2);

                    rand = rng->nextInt(0, zone2Tiles.size() - 1);
                    auto [outerXXXX2, outerYYYY2] = zone2Tiles[rand];

                    int outerXX2 = outerXXXX2;
                    int outerYY2 = outerYYYY2;

                    entrance2.setPosition(int3(outerXX2, outerYY2, 0)); // Fix position
                    connectedPairs.emplace_back(XX, YY, outerXX2, outerYY2, true, rng->nextInt(1,3)); //Castle2 -> Entrance2
                    map.getTile(outerXX2, outerYY2+1)->setIsGuard(true);
                    map.getTile(outerXX2, outerYY2+1)->setIsBorder(false);



                    placeMonolith(entrance2);
                    mapObjects.emplace_back(entrance2);

                    //Create second exit
                    Object exit2(int3 (0, 0, 0), "Monolith One Way Exit" + std::to_string(monolithCount + 1));
                    size = monolithCount > 2 ? int3(2, 1, 0) : int3(1, 1, 0);
                    exit2.setSizeOfObject(size);
                    
                    zone1Tiles = getValidTiles(zone1Id, exit2);

                    rand = rng->nextInt(0, zone1Tiles.size() - 1);
                    auto [outerXXXX1, outerYYYY1] = zone1Tiles[rand];

                    int outerXX1 = outerXXXX1;
                    int outerYY1 = outerYYYY1;

                    exit2.setPosition(int3(outerXX1, outerYY1, 0)); // Fix position
                    connectedPairs.emplace_back(X, Y, outerXX1, outerYY1, true, rng->nextInt(1,3)); //Castle1 -> Exit2

                    placeMonolith(exit2);
                    mapObjects.emplace_back(exit2);

                    monolithCount += 2;
                    monolithCount %= 8;
                }
                else
                {
                    connectedPairs.emplace_back(outerX1, outerY1, outerX2, outerY2, false, tier); // Add points of connection
                    connectedPairs.emplace_back(outerX1, outerY1, X, Y, true, tier); // Castle1 -> Connect1
                    connectedPairs.emplace_back(XX, YY, outerX2, outerY2, true, tier); // Castle2 -> Connect2

                    map.getTile(outerX1,outerY1)->setIsGate(true);
                    map.getTile(outerX2,outerY2)->setIsGate(true);

                    if(debug){
                        std::cerr << "X= " << X << " Y= " << Y << " OX1= " << outerX1 << " OY1= " << outerY1 << " OX2= " << outerX2 << " OY2= " << outerY2 << "\n";
                        for (int y = 0; y < mapHeight; y++)
                        {
                            for (int x = 0; x < mapWidth; x++) {
                                auto TilePtr = map.getTile(x, y);

                        
                                if((x == outerX1 && y == outerY1) || (x == outerX2 && y == outerY2)) {
                                    std::cerr << "X ";
                                } else if((x == X && y == Y) || (x == XX && y == YY)) {
                                    std::cerr << "Y ";
                                }else if(!TilePtr->getIsGate() && TilePtr->getIsBorder()){
                                    std::cerr << "E ";
                                }
                                else
                                {
                                    std::cerr << ". ";
                                }
                            }
                            std::cerr << "\n";
                        }
                        
                        for(auto myTuple : getConnectedPairs()){
                            std::cerr << "(" 
                                << std::get<0>(myTuple) << ", "
                                << std::get<1>(myTuple) << ", "
                                << std::get<2>(myTuple) << ", "
                                << std::get<3>(myTuple) << ")\n";
                        }
                    }
                }
            }
        }
    }
}

bool BorderPlacer::isWide(int ZoneA, int ZoneB){
    auto connectionPair = std::make_pair(std::min(ZoneA, ZoneB), std::max(ZoneA, ZoneB));
    return (wideConnections.find(connectionPair) == wideConnections.end());
}

void BorderPlacer::setWideConnections(){
    auto zonesI = temp.getZonesI();

    for(auto e : zonesI){
        for(auto c : e.second->getConnections()){
            if(c.getType() == "wide"){
                auto connectionPair = std::make_pair(std::min(c.getZoneA(), c.getZoneB()), std::max(c.getZoneA(), c.getZoneB()));
                this->wideConnections.insert(connectionPair);
            }
        }
    }

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            auto TilePtr = map.getTile(x, y);

            int currentZoneId = TilePtr->getZoneId();
    
            for (int i = 0; i < 8; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                auto NeighborTilePtr = map.getTile(nx, ny);

                if (NeighborTilePtr && NeighborTilePtr->getZoneId() != currentZoneId && !isWide(NeighborTilePtr->getZoneId(), currentZoneId)) {
                    TilePtr->setIsBorder(false);
                    break;
                }
            }

            if (debug) {
                std::cerr << "Border Tile: (" << x << ", " << y << ")\n";
            }
        }
    }
}

void BorderPlacer::determineZoneBorders() {
    if (debug) std::cerr << "Determining zone Borders\n";

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            auto TilePtr = map.getTile(x, y);

            int currentZoneId = TilePtr->getZoneId();
            bool isBorder = false;
    
            for (int i = 0; i < 8; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                auto NeighborTilePtr = map.getTile(nx, ny);

                if (NeighborTilePtr && NeighborTilePtr->getZoneId() != currentZoneId) {
                    isBorder = true;

                    if(!isMapBorder(x, y)){
                        NeighborTilePtr->setNeighbourZones(currentZoneId);
                        TilePtr->setNeighbourZones(NeighborTilePtr->getZoneId());
                    }
                }
            }

            TilePtr->setIsBorder(isBorder);

            if (debug) {
                std::cerr << "Border Tile: (" << x << ", " << y << ")\n";
            }
        }
    }
}
