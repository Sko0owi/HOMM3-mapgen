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

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    std::queue<std::pair<int, int>> q;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;

    q.emplace(x1, y1);
    cameFrom[{x1, y1}] = {-1, -1}; 

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        if (cx == x2 && cy == y2) {
            while (cameFrom[{cx, cy}] != std::make_pair(-1, -1)) {
                path.emplace_back(cx, cy);
                std::tie(cx, cy) = cameFrom[{cx, cy}];
            }
            path.emplace_back(x1, y1);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            auto TilePtr = map.getTile(nx, ny);

            //!TilePtr->getIsBorder() &&
            if (TilePtr && cameFrom.find({nx, ny}) == cameFrom.end() && 
                    (!TilePtr->getIsGate() || (TilePtr->getIsGate() && map.isMiddle(nx, ny)))) {
                q.emplace(nx, ny);
                cameFrom[{nx, ny}] = {cx, cy};
            }
        }
    }

    return path;
}

void RoadPlacer::createShotestPathsToConnected(std::ofstream& luaFile, std::vector<std::tuple<int, int, int, int, bool>> &connectedPairs) {
    auto zonesI = temp.getZonesI();
    std::set<std::pair<int, int>> processedConnections;

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};



    luaFile << "-- Dynamic terrain adjustments for linear paths between towns\n";
    luaFile << "instance:terrain(function (x, y, z)\n";

    for(auto e : connectedPairs){
        auto [x1, y1, x2, y2, castle] = e;
        auto path = generateSimplePath(x1, y1, x2, y2);

        int i = 0, max = path.size();
        for (const auto &point : path)
        {
            auto TilePtr = map.getTile(point.first, point.second);
            TilePtr->setIsBorder(false);

            if(i == max-2 && !castle){
                TilePtr->setIsMiddleGate(true);
            }
            i++;

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
