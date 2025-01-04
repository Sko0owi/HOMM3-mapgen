#include <bits/stdc++.h>


class Tile {
public: 


    Tile();
    void setZoneId(int zoneId);
    int getZoneId();
    void setIsEdge(bool isEdge);
    bool getIsEdge();


private:
    int zoneId;
    bool isEdge;
};