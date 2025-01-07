
#include <bits/stdc++.h>


class Map;
class TemplateInfo;


class BorderPlacer {
public: 
    BorderPlacer(Map & map, TemplateInfo & temp);

    void generateBorders();

    bool areConnected(int ZoneA, int ZoneB);
    int getTier(int ZoneA, int ZoneB);
    bool isMapBorder(int x, int y);

    void findOuter(int X, int Y, int *outerX1, int *outerY1, int *outerX2, int *outerY2, int zone1Id, int zone2Id);
    bool isWide(int ZoneA, int ZoneB);
    void determineZoneBorders();
    void connectZones();

    void createExtenstion();
    void removeExtension();

    bool roadInRange(int range = 1);
    void fixBorders();

    std::vector<std::tuple<int, int, int, int, bool, int>> getConnectedPairs();
    void setWideConnections();

private:
    int mapWidth;
    int mapHeight;
    bool debug = false;

    static constexpr int dx[] = {-1, 1, 0, 0};
    static constexpr int dy[] = {0, 0, -1, 1};

    Map &map;
    TemplateInfo &temp;
    std::set<std::pair<int, int>> wideConnections;

    std::vector<std::tuple<int, int, int, int, bool, int>> connectedPairs;
};