
#include <bits/stdc++.h>


class Map;
class TemplateInfo;


class RoadPlacer {
public:
    RoadPlacer();
    std::vector<std::pair<int, int>> generateSimplePath(int x1, int y1, int x2, int y2, Map &map);
    void createShotestPathsToConnected(std::ofstream &luaFile, std::vector<std::tuple<int, int, int, int, bool>> &connectedPairs, Map &map, TemplateInfo &temp);

private:
    std::map<int, std::map<int, size_t>> DistancesBetweenZones;
};
