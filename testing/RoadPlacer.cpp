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

void RoadPlacer::createShotestPathsToConnected(std::ofstream& luaFile, std::vector<std::tuple<int, int, int, int, bool>> &connectedPairs) {
    auto zonesI = temp.getZonesI();
    std::set<std::pair<int, int>> processedConnections;

    luaFile << "-- Dynamic terrain adjustments for linear paths between towns\n";
    luaFile << "instance:terrain(function (x, y, z)\n";

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

            luaFile << "    if x == " << point.first << " and y == " << point.second << " then return nil, 3 end\n";
        }
    }
    luaFile << "    return nil\n"; // Default terrain
    luaFile << "end)\n";
}


