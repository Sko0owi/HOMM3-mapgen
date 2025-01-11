#include "./BorderPlacer.h"
#include "./Map.h"
#include "./Zone.h"
#include "./Tile.h"
#include "../template_info/TemplateInfo.h"
#include "../template_info/ZoneInfo.h"
#include "../template_info/ConnectionInfo.h"
#include "../global/Random.h"
#include"../types/int3.h"
#include "../game_info/Object.h"

TemplateInfo emptyTemplateInfoBorder;

BorderPlacer::BorderPlacer(Map &map, TemplateInfo &temp, RNG *rng) : map(map), temp(temp), rng(rng)
{
    mapWidth = 0;
    mapHeight = 0;
    monolithCount = 0;
}

BorderPlacer::BorderPlacer(Map & map, RNG *rng) 
    : BorderPlacer(map, emptyTemplateInfoBorder, rng) {}

void BorderPlacer::generateBorders() {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    determineZoneBorders();
    createExtenstion();
    connectZones();
    setWideConnections();
    removeExtension();
}

void BorderPlacer::createExtenstion(){
    for (int y = 0; y < mapHeight; y++){
        for (int x = 0; x < mapWidth; x++) {
            auto TilePtr = map.getTile(x, y);

            for (int i = 0; i < 4; i++){
                int nx = x + dx[i];
                int ny = y + dy[i];

                auto NeighborTilePtr = map.getTile(nx, ny);

                if(!TilePtr->getIsBorder() && NeighborTilePtr && NeighborTilePtr->getIsBorder()){
                    TilePtr->setIsExtension(true);
                }
            }
        }
    }
}

void BorderPlacer::removeExtension(){
    for (int y = 0; y < mapHeight; y++){
        for (int x = 0; x < mapWidth; x++) {
            auto TilePtr = map.getTile(x, y);

            bool found = false;
            for (int i = 0; i < 4; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                auto NeighborTilePtr = map.getTile(nx, ny);

                if(TilePtr->getIsExtension() && NeighborTilePtr && NeighborTilePtr->getIsBorder()){
                    found = true;
                }
            }
            TilePtr->setIsExtension(found);
        }
    }
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
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++){
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

            // if(TilePtr->getIsExtension()) //We would want to run from extension tiles
            //     newDist += 1.0;

            if (newDist < distance[ny][nx])
                distance[ny][nx] = newDist;
            else 
                continue;
            

            if(zone1Id == ZoneC && !TilePtr->getIsBorder()){ // We didn't get out of zone
                pq.emplace(nx, ny, -1, -1, newDist);
            } else if (zone2Id == ZoneC && !TilePtr->getIsBorder()) { // We found good zone 
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
            { // We get out of zone
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

std::vector<std::pair<int, int>> BorderPlacer::getZoneTiles(int zoneId){
    std::vector<std::pair<int, int>> tiles;
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++){
            auto TilePtr = map.getTile(x, y);

            if(TilePtr->getZoneId() == zoneId && !TilePtr->getIsBorder() && !TilePtr->getIsExtension() && !TilePtr->getIsRoad()){
                tiles.emplace_back(x, y);
            }
        }
    }
    return tiles;
}

void BorderPlacer::connectZones() {
    std::cerr << "Finding zones conenction points\n";
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
                int tier = getTier(zone1Id, zone2Id);
                std::string type = getType(zone1Id, zone2Id);

                int outerX1 = -1, outerY1 = -1, outerX2 = -1, outerY2 = -1;
                findOuter(X, Y, &outerX1, &outerY1, &outerX2, &outerY2, zone1Id, zone2Id);
                // OuterXi -> outer X coord from zonei
                // OuterYi -> outer Y coord from zone1
                // castle -> bool does have castle
                // tier -> tier of road

                if(outerX1 == -1 || outerX2 == -1 || type == "monolith"){ // Didn't find connection or it is monolith type -> add Monoliths
                    std::vector<std::pair<int, int>> zone1Tiles = getZoneTiles(zone1Id);
                    std::vector<std::pair<int, int>> zone2Tiles = getZoneTiles(zone2Id);

                    int rand = rng->nextInt(0, zone1Tiles.size() - 1), rand2;
                    auto [outerXX1, outerYY1] = zone1Tiles[rand];

                    outerX1 = outerXX1;
                    outerY1 = outerYY1;

                    Object entrance(int3 (outerX1, outerY1, 0), "Monolith One Way Entrance" + std::to_string(monolithCount));
                    int3 size = monolithCount > 3 ? int3(2, 2, 0) : int3(1, 2, 0);
                    entrance.setSizeOfObject(size);
                    mapObjects.emplace_back(entrance);

                    //Now we need to create connection in other direction as well
                    do{
                        rand2 = rng->nextInt(0, zone1Tiles.size() - 1);
                    } while (rand == rand2);
                    
                    auto [outerXXX1, outerYYY1] = zone1Tiles[rand2];

                    //First exit 
                    rand = rng->nextInt(0, zone2Tiles.size() - 1);
                    auto [outerXX2, outerYY2] = zone2Tiles[rand];

                    outerX2 = outerXX2;
                    outerY2 = outerYY2;

                    
                    Object exit(int3 (outerX2, outerY2, 0), "Monolith One Way Exit" + std::to_string(monolithCount));
                    size = monolithCount > 3 ? int3(2, 2, 0) : int3(1, 2, 0);
                    exit.setSizeOfObject(size);
                    mapObjects.emplace_back(exit);

                    //Now we need to create connection in other direction as well
                    do{
                        rand2 = rng->nextInt(0, zone2Tiles.size() - 1);
                    } while (rand == rand2);
                    
                    auto [outerXXX2, outerYYY2] = zone2Tiles[rand2];
                    Object entrance2(int3 (outerXXX2, outerYYY2, 0), "Monolith One Way Entrance" + std::to_string(monolithCount + 1));
                    size = monolithCount > 2 ? int3(2, 2, 0) : int3(1, 2, 0);
                    entrance2.setSizeOfObject(size);
                    mapObjects.emplace_back(entrance2);
                    
                    // int XX2 = zone2.second->getPosition().x;
                    // int YY2 = zone2.second->getPosition().y;

                    // connectedPairs.emplace_back(outerXXX2, outerYYY2, XX2, YY2, true, tier); // Castle2 -> Connect2

                    //Second exit
                    Object exit2(int3 (outerXXX1, outerYYY1, 0), "Monolith One Way Exit" + std::to_string(monolithCount + 1));
                    size = monolithCount > 2 ? int3(2, 2, 0) : int3(1, 2, 0);
                    exit2.setSizeOfObject(size);
                    mapObjects.emplace_back(exit2);
                    
                    // int XX1 = zone1.second->getPosition().x;
                    // int YY1 = zone1.second->getPosition().y;

                    // connectedPairs.emplace_back(outerXXX1, outerYYY1, XX1, YY1, true, tier); // Castle1 -> Connect1
                    
                    monolithCount += 2;
                    monolithCount %= 8;
                }
                else
                {
                    connectedPairs.emplace_back(outerX1, outerY1, outerX2, outerY2, false, tier); // Add points of connection

                    // map.getTile((outerX1), (outerY1))->setIsGate(true);

                    int XX1 = zone1.second->getPosition().x;
                    int YY1 = zone1.second->getPosition().y;


                    connectedPairs.emplace_back(outerX1, outerY1, XX1, YY1, true, tier); // Castle1 -> Connect1
                    
                    int XX2 = zone2.second->getPosition().x;
                    int YY2 = zone2.second->getPosition().y;
                    connectedPairs.emplace_back(XX2, YY2, outerX2, outerY2, true, tier); // Castle2 -> Connect2

                
                    if(debug){
                        std::cerr << "X= " << X << " Y= " << Y << " OX1= " << outerX1 << " OY1= " << outerY1 << " OX2= " << outerX2 << " OY2= " << outerY2 << "\n";
                        for (int y = 0; y < mapHeight; y++)
                        {
                            for (int x = 0; x < mapWidth; x++) {
                                auto TilePtr = map.getTile(x, y);

                        
                                if((x == outerX1 && y == outerY1) || (x == outerX2 && y == outerY2)) {
                                    std::cerr << "X ";
                                } else if((x == XX1 && y == YY1) || (x == XX2 && y == YY2)) {
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
    
            for (int i = 0; i < 4; i++)
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
    std::cerr << "Determining zone Borders\n";

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            auto TilePtr = map.getTile(x, y);

            int currentZoneId = TilePtr->getZoneId();
            bool isBorder = false;
    
            for (int i = 0; i < 4; i++)
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

bool BorderPlacer::roadInRange(int range){
    return true;
}
