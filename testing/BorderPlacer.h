
#include <bits/stdc++.h>


class Map;
class TemplateInfo;


class BorderPlacer {
public: 
    BorderPlacer(Map & map, TemplateInfo & temp);

    void generateBorders();

    bool areConnected(int ZoneA, int ZoneB);
    bool isMapBorder(int x, int y);
    
    void findOuter(int X, int Y, int *outerX1, int *outerY1, int *outerX2, int *outerY2, int zone1Id, int zone2Id);
    void determineZoneBorders();
    void connectZones();
    
    std::vector<std::tuple<int, int, int, int, bool>> getConnectedPairs();
    
private:
    int mapWidth;
    int mapHeight;
    bool debug = false;

    Map & map;
    TemplateInfo & temp;

    std::vector<std::tuple<int, int, int, int, bool>> connectedPairs;
};