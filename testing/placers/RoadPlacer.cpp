#include "./RoadPlacer.h"
#include "../template_info/TemplateInfo.h"
#include "../template_info/ZoneInfo.h"
#include "../template_info/ConnectionInfo.h"
#include "./ObjectPlacer.h"
#include "./Map.h"
#include "./Tile.h"

RoadPlacer::RoadPlacer(Map &map, TemplateInfo &temp, std::shared_ptr<ObjectPlacer> objectPlacer) : map(map), temp(temp), objectPlacer(objectPlacer) {};

std::vector<std::pair<int, int>> RoadPlacer::generateSimplePath(int x1, int y1, int x2, int y2) {
    std::vector<std::pair<int, int>> path;
    auto objectsMap = objectPlacer->getObjectsMap();
    
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

            auto TilePtr = map.getTile(nx, ny);

            if (TilePtr && (!TilePtr->getIsGate() || (TilePtr->getIsGate() && map.isMiddle(nx, ny))) && objectsMap[nx][ny] <= 1) {
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

void RoadPlacer::fixBorders(){
    for (int x = 0; x < map.getWidth() - 1; x++){
        for (int y = 0; y < map.getHeight() - 1; y++){
            int cnt = 0;

            auto Tile = map.getTile(x, y);

            if(!(Tile->getIsExtension() || Tile->getIsBorder())){ // If is empty
                for (int i = 0; i < 4; i++){
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    auto TileNeigbour = map.getTile(nx, ny);

                    if(TileNeigbour && (TileNeigbour->getIsExtension() || TileNeigbour->getIsBorder())){
                        cnt++;
                    }
                }
            }

            if(cnt >= 3)
                Tile->setIsBorder(true);
        }
    }
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

bool RoadPlacer::gateSquare(int x, int y){
    auto Tile1 = map.getTile(x + 1, y), Tile2 = map.getTile(x - 1, y);
    if(Tile1 && Tile2 && (Tile1->getIsBorder() || Tile1->getIsExtension()) && (Tile2->getIsBorder() || Tile2->getIsExtension())){
        // std::cerr << x << " " << y << " 1\n";
        return true;
    }

    Tile1 = map.getTile(x, y + 1), Tile2 = map.getTile(x, y - 1);
    if(Tile1 && Tile2 && (Tile1->getIsBorder() || Tile1->getIsExtension()) && (Tile2->getIsBorder() || Tile2->getIsExtension())){
        // std::cerr << x << " " << y << " 2\n";
        return true;
    }

    Tile1 = map.getTile(x + 1, y + 1), Tile2 = map.getTile(x - 1, y - 1);
    if(Tile1 && Tile2 && (Tile1->getIsBorder() || Tile1->getIsExtension()) && (Tile2->getIsBorder() || Tile2->getIsExtension())){
        // std::cerr << x << " " << y << " 3\n";
        return true;
    }

    Tile1 = map.getTile(x + 1, y - 1), Tile2 = map.getTile(x - 1, y + 1);
    if(Tile1 && Tile2 && (Tile1->getIsBorder() || Tile1->getIsExtension()) && (Tile2->getIsBorder() || Tile2->getIsExtension())){
        // std::cerr << x << " " << y << " 4\n";
        return true;
    }

    return false;
}

void RoadPlacer::createShotestPathsToConnected(std::vector<std::tuple<int, int, int, int, bool, int>> &connectedPairs) {
    auto zonesI = temp.getZonesI();
    std::set<std::pair<int, int>> processedConnections;
    
    fixBorders();

    std::cerr << "ROAD PLACER POV\n";
    auto objectsMap = objectPlacer->getObjectsMap();
    for (int x = 0; x < map.getWidth(); x++){
        for (int y = 0; y < map.getHeight(); y++){
            if(objectsMap[x][y] > 1)
                std::cerr << objectsMap[x][y] << " ";
            else
                std::cerr << ". ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n\n";

    for(auto e : connectedPairs){
        auto [x1, y1, x2, y2, castle, tier] = e;
        auto path = generateSimplePath(x1, y1, x2, y2);

        for (const auto &point : path)
        {
            auto TilePtr = map.getTile(point.first, point.second);
            TilePtr->setIsBorder(false);
            TilePtr->setIsExtension(false);
            TilePtr->setIsRoad(true);
            TilePtr->setTier(tier);
        }

        if(!castle){
            for (const auto &point : path)
            {
                auto TilePtr = map.getTile(point.first, point.second);
                if(gateSquare(point.first, point.second)){
                    TilePtr->setIsGate(true);
                    break;
                }

            }
        }
    }
    
    for (int x = 0; x < map.getWidth(); x++){
        for (int y = 0; y < map.getHeight(); y++){
            auto TilePtr = map.getTile(y, x);

            if(TilePtr->getIsBorder()){
                std::cerr << "E ";
            } else if(TilePtr->getIsRoad()){
                std::cerr << "R ";
            }
            else
                std::cerr << ". ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";

    clearSquares();
}
