#include "./RoadPlacer.h"
#include "./Template.h"
#include "./Map.h"
#include "./Tile.h"

RoadPlacer::RoadPlacer(Map &map, TemplateInfo &temp) : map(map), temp(temp) {};

std::vector<std::pair<int, int>> RoadPlacer::generateSimplePath(int x1, int y1, int x2, int y2) {
    std::vector<std::pair<int, int>> path;

    if (x1 == x2 && y1 == y2) {
        path.emplace_back(x1, y1);
        return path;
    }

    int mapHeight = map.getHeight();
    int mapWidth = map.getWidth();

    std::vector<std::vector<int>> distance(mapHeight, std::vector<int>(mapWidth, std::numeric_limits<int>::max()));
    distance[y1][x1] = 0;

    std::queue<std::vector<std::pair<int, int>>> q;
    q.push({{x1, y1}}); 

    while (!q.empty()) {
        std::vector<std::pair<int, int>> currentPath = q.front();
        q.pop();

        auto [cx, cy] = currentPath.back();

        if (cx == x2 && cy == y2) {
            return currentPath;
        }

        for (int i = 0; i < 8; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight) continue;

            auto TilePtr = map.getTile(nx, ny);

            if (TilePtr && (!TilePtr->getIsGate() || (TilePtr->getIsGate() && map.isMiddle(nx, ny)))) {
                int newDistance = distance[cy][cx] + 1; 

                if (newDistance < distance[ny][nx]) {
                    distance[ny][nx] = newDistance;

                    std::vector<std::pair<int, int>> newPath = currentPath;

                    if (i == 4) { // LU
                        newPath.emplace_back(cx, cy + dy[i]);
                    } else if (i == 5) { // LD 
                        newPath.emplace_back(cx + dx[i], cy);
                    } else if (i == 6) { // RU
                        newPath.emplace_back(cx + dx[i], cy);
                    } else if (i == 7) { // RD
                        newPath.emplace_back(cx, cy + dy[i]);
                    }

                    newPath.emplace_back(nx, ny); 
                    q.push(newPath);
                }
            }
        }
    }

    return path;
}

void RoadPlacer::writeRoadsToFile(std::ofstream& luaFile){
    luaFile << "-- Dynamic terrain adjustments for linear paths between towns\n";
    luaFile << "instance:terrain(function (x, y, z)\n";

    for (int x = 0; x < map.getWidth(); x++){
        for (int y = 0; y < map.getHeight(); y++){
            auto TilePtr = map.getTile(y, x);
            if (TilePtr->getIsRoad()) {            
                luaFile << "    if x == " << y << " and y == " << x << " then return nil, 3 end\n";
                std::cerr << "R ";
            } else{
                std::cerr << ". ";
            }
        }
        std::cerr << "\n";
    }

    luaFile << "    return nil\n"; // Default terrain
    luaFile << "end)\n";
}

void RoadPlacer::clearSquares(){
    // TODO PLS DO IT IN SMART 100% WORKING WAY
    for (int x = 0; x < map.getWidth() - 1; x++){
        for (int y = 0; y < map.getHeight() - 1; y++){
            bool found = true;

            found = (map.getTile(x, y)->getIsRoad() && map.getTile(x + 1, y)->getIsRoad() && map.getTile(x, y + 1)->getIsRoad() && map.getTile(x + 1, y + 1)->getIsRoad());

            if(found){

                auto Tile1 = map.getTile(x, y - 1), Tile2 = map.getTile(x, y + 2), Tile3 = map.getTile(x, y - 1);
                if(Tile1 && Tile2 && Tile1->getIsRoad() && Tile2->getIsRoad()){
                    Tile3 = map.getTile(x + 2, y);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x + 1, y + 1)->setIsRoad(false);
                        continue;
                    } 

                    Tile3 = map.getTile(x + 2, y + 1);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x + 1, y)->setIsRoad(false);
                        continue;
                    } 


                    map.getTile(x + 1, y)->setIsRoad(false);
                    map.getTile(x + 1, y + 1)->setIsRoad(false);
                    continue;
                }

                Tile1 = map.getTile(x + 1, y - 1), Tile2 = map.getTile(x + 1, y + 2);
                if(Tile1 && Tile2 && Tile1->getIsRoad() && Tile2->getIsRoad()){
                    Tile3 = map.getTile(x - 1, y);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x, y + 1)->setIsRoad(false);
                        continue;
                    } 

                    Tile3 = map.getTile(x - 1, y + 1);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x, y)->setIsRoad(false);
                        continue;
                    } 

                    map.getTile(x, y)->setIsRoad(false);
                    map.getTile(x, y + 1)->setIsRoad(false);
                    continue;
                }

                Tile1 = map.getTile(x - 1, y), Tile2 = map.getTile(x + 2, y);
                if(Tile1 && Tile2 && Tile1->getIsRoad() && Tile2->getIsRoad()){
                    Tile3 = map.getTile(x + 1, y + 2);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x, y + 1)->setIsRoad(false);
                        continue;
                    } 

                    Tile3 = map.getTile(x, y + 2);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x + 1, y + 1)->setIsRoad(false);
                        continue;
                    } 

                    map.getTile(x, y + 1)->setIsRoad(false);
                    map.getTile(x + 1, y + 1)->setIsRoad(false);
                    continue;
                }

                Tile1 = map.getTile(x - 1, y + 1), Tile2 = map.getTile(x + 2, y + 1);
                if(Tile1 && Tile2 && Tile1->getIsRoad() && Tile2->getIsRoad()){
                    Tile3 = map.getTile(x, y - 1);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x + 1, y)->setIsRoad(false);
                        continue;
                    } 

                    Tile3 = map.getTile(x + 1, y - 1);
                    if(Tile3 && Tile3->getIsRoad()){
                        map.getTile(x, y)->setIsRoad(false);
                        continue;
                    } 

                    map.getTile(x, y)->setIsRoad(false);
                    map.getTile(x + 1, y)->setIsRoad(false);
                    continue;
                }
            }
        }
    }
}

void RoadPlacer::createShotestPathsToConnected(std::ofstream& luaFile, std::vector<std::tuple<int, int, int, int, bool>> &connectedPairs) {
    auto zonesI = temp.getZonesI();
    std::set<std::pair<int, int>> processedConnections;
    
    for(auto e : connectedPairs){
        auto [x1, y1, x2, y2, castle] = e;
        auto path = generateSimplePath(x1, y1, x2, y2);

        int j = 0, max = path.size();
        for (const auto &point : path)
        {
            auto TilePtr = map.getTile(point.first, point.second);
            TilePtr->setIsBorder(false);
            TilePtr->setIsRoad(true);

            if(j == max/2 && !castle){
                TilePtr->setIsGate(true);
            }
            j++;

            for (int i = 0; i < 4; ++i) {
                int nx = point.first + dx[i];
                int ny = point.second + dy[i];

                auto TilePtr = map.getTile(nx, ny);

                if (TilePtr && TilePtr->getIsBorder()) {
                    TilePtr->setIsBorder(false);
                }
            }

        }
    }

    clearSquares();
    writeRoadsToFile(luaFile);
}


