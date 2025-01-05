#include "./BorderPlacer.h"
#include "./Map.h"
#include "./Zone.h"
#include "./Tile.h"
#include "./Template.h"


BorderPlacer::BorderPlacer(Map & map, TemplateInfo & temp) : map(map), temp(temp){
    mapWidth = 0;
    mapHeight = 0;
}

void BorderPlacer::generateBorders() {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    determineZoneBorders();
    connectZones();
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
    std::queue<std::tuple<int, int, int, int>> q;
    std::set<std::pair<int, int>> visited;

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    q.emplace(X, Y, -1, -1);
    visited.insert({X, Y});

    while (!q.empty()) {
        auto [cx, cy, fromx, fromy] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (visited.count({nx, ny})) continue;
        
            auto TilePtr = map.getTile(nx, ny);
            int ZoneC = -1;
            if(TilePtr)
                ZoneC = TilePtr->getZoneId();

            if(zone1Id == ZoneC && !TilePtr->getIsBorder()){ // We didn't get out of zone
                q.emplace(nx, ny, -1, -1);
            } else if (zone2Id == ZoneC && !TilePtr->getIsBorder()) { // We found good zone 
                (*outerX1) = fromx;
                (*outerY1) = fromy;
                (*outerX2) = nx;
                (*outerY2) = ny;
                return;
            }
            else if (((zone1Id == ZoneC && TilePtr->getIsBorder()) || (zone2Id == ZoneC && TilePtr->getIsBorder())) && fromx != -1)
            { // Travelling through Border
                q.emplace(nx, ny, fromx, fromy);
            }
            else
            { // We get out of zone
                if(ZoneC == zone1Id || ZoneC == zone2Id)
                    q.emplace(nx, ny, cx, cy);
            }

            visited.insert({nx, ny});
        }
    }
}

std::vector<std::tuple<int, int, int, int, bool>> BorderPlacer::getConnectedPairs(){
    return connectedPairs;
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

                int outerX1 = -1, outerY1 = -1, outerX2 = -1, outerY2 = -1;

                findOuter(X, Y, &outerX1, &outerY1, &outerX2, &outerY2, zone1Id, zone2Id);
                connectedPairs.emplace_back(outerX1, outerY1, outerX2, outerY2, false); // Add points of connection

                // map.getTile((outerX1), (outerY1))->setIsGate(true);

                int XX1 = zone1.second->getPosition().x;
                int YY1 = zone1.second->getPosition().y;

                connectedPairs.emplace_back(outerX1, outerY1, XX1, YY1, true); // Castle1 -> Connect1
                
                int XX2 = zone2.second->getPosition().x;
                int YY2 = zone2.second->getPosition().y;
                connectedPairs.emplace_back(XX2, YY2, outerX2, outerY2, true); // Castle2 -> Connect2

                
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

void BorderPlacer::determineZoneBorders() {
    std::cerr << "Determining zone Borders\n";

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            auto TilePtr = map.getTile(x, y);

            int currentZoneId = TilePtr->getZoneId();
            bool isBorder = false;
    
            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};
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