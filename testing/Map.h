#include<bits/stdc++.h>
#include "../global/Global.h"

class Zone;
class TemplateInfo;

using MapZones = std::map<i32, std::shared_ptr<Zone>>;
class Map {
public:
    Map();
    Map(i32 width, i32 height);
    ~Map();
    void initialize(TemplateInfo &temp);
    void generate();
    void print();
    MapZones & getZones();
    i32 getWidth();
    i32 getHeight();


private:
    i32 width;
    i32 height;
    MapZones zones;

};