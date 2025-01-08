
#include <bits/stdc++.h>


class Map;
class TemplateInfo;


class RoadPlacer {
public:
    RoadPlacer(Map & map, TemplateInfo & temp);
    
    std::vector<std::pair<int, int>> generateSimplePath(int x1, int y1, int x2, int y2);
    void createShotestPathsToConnected(std::vector<std::tuple<int, int, int, int, bool, int>> &connectedPairs);
    
    void clearSquares();
    void fixBorders();
    bool gateSquare(int x, int y);

private:
    std::map<int, std::map<int, size_t>> DistancesBetweenZones;

    Map & map;
    TemplateInfo & temp;

    static constexpr int dx[] = {0, -1, 1, 0, -1, -1, 1, 1};
    static constexpr int dy[] = {1, 0, 0, -1, -1, 1, -1, 1};
};
